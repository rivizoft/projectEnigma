#include "Window.h"
#include <sstream>

Window::WindowClass Window::WindowClass::wndClass;

Window::Window(unsigned short int width, unsigned short int height, const char* title)
{
	throw Exception(__LINE__, __FILE__, ERROR_ACCESS_DENIED);

	hWnd = CreateWindow(
		WindowClass::getClassName(),
		title,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width,
		height,
		nullptr,
		nullptr,
		WindowClass::getInstance(),
		this
	);

	ShowWindow(hWnd, SW_SHOWDEFAULT);
}

Window::~Window()
{
	DestroyWindow(hWnd);
}

LRESULT Window::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_NCCREATE)
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		return pWnd->HandleMessage(hWnd, msg, wParam, lParam);
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	return pWnd->HandleMessage(hWnd, msg, wParam, lParam);
}

Window::WindowClass::WindowClass() noexcept : hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEX window;
	window.style = CS_OWNDC;
	window.lpfnWndProc = HandleMsgSetup;
	window.cbClsExtra = 0;
	window.cbWndExtra = 0;
	window.hInstance = getInstance();
	window.hIcon = nullptr;
	window.hCursor = nullptr;
	window.hbrBackground = CreateSolidBrush(RGB(0, 255, 9));
	window.lpszMenuName = nullptr;
	window.lpszClassName = getClassName();
	window.hIconSm = nullptr;
	window.cbSize = sizeof(window);
	
	RegisterClassEx(&window);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(wndClassName, getInstance());
}

const char* Window::WindowClass::getClassName() noexcept
{
	return wndClassName;
}

HINSTANCE Window::WindowClass::getInstance() noexcept
{
	return wndClass.hInst;
}

Window::Exception::Exception(int line, const char* file, HRESULT hr) : EngineException(line, file)
{
	this->hr = hr;
}

const char* Window::Exception::what() const
{
	std::ostringstream oss;
	oss << getType() << std::endl
		<< "Error Code: " << getErrorCode() << std::endl
		<< "Details: " << getErrorString() << std::endl
		<< getOriginString();

	return oss.str().c_str();
}

const char* Window::Exception::getType() const
{
	return "Enigma Engine Window Exception";
}

std::string Window::Exception::TranslateErrorCode(HRESULT hr)
{
	char* pMsgBuffer = nullptr;
	DWORD nMsgLength = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		hr,
		LANG_SYSTEM_DEFAULT,
		reinterpret_cast<LPSTR>(&pMsgBuffer),
		0,
		nullptr
	);

	if (nMsgLength == 0)
		return "Undefined error code.";

	std::string errorString = pMsgBuffer;
	LocalFree(pMsgBuffer);
	return errorString;
}

HRESULT Window::Exception::getErrorCode() const
{
	return this->hr;
}

std::string Window::Exception::getErrorString() const
{
	return TranslateErrorCode(this->hr);
}

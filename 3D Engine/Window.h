#pragma once
#include "Framework.h"
#include "EngineException.h"

#define EXCEPT(hr) Window::Exception(__LINE__, __FILE__, hr);

class Window 
{
public: 
	class Exception : public EngineException
	{
	public: 
		Exception(int line, const char* file, HRESULT hr);
		const char* what() const override;
		virtual const char* getType() const;
		static std::string TranslateErrorCode(HRESULT hr);
		HRESULT getErrorCode() const;
		std::string getErrorString() const;

	private:
		HRESULT hr;
	};

private: 
	class WindowClass
	{
	private: 
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr const char* wndClassName = "DirectX Engine";
		static WindowClass wndClass;
		HINSTANCE hInst;

	public:
		static const char* getClassName() noexcept;
		static HINSTANCE getInstance() noexcept;
	};

public:
	Window(unsigned short int width, 
		unsigned short int height, 
		const char* title);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	unsigned short int _width;
	unsigned short int _height;
	HWND hWnd;
};


#include "Window.h"

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR	  lpCmdLine,
	int		  nCmdShow)
{
	try {
		Window win(500, 500, "3D Engine");

		MSG msg;
		BOOL result;
		while ((result = GetMessageA(&msg, nullptr, 0, 0)) != 0)
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}

		if (result == -1)
			return -1;

		return msg.wParam;
	}
	catch (const EngineException& e) {
		MessageBoxA(nullptr, e.what(), e.getType(), MB_OK | MB_ICONERROR);
	}
	catch (const std::exception & e) {
		MessageBoxA(nullptr, e.what(), "Standart Exception", MB_OK | MB_ICONERROR);
	}

	return 0;
}



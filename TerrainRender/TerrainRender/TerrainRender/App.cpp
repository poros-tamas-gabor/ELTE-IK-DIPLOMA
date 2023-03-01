#include "App.h"
#include <windows.h>

App::App(){}
App::~App() {};

LRESULT CALLBACK App::WindowProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	//Keyboard Messages

	// Check if a key has been pressed on the keyboard.
	case WM_KEYDOWN:
	{
		unsigned char keycode = static_cast<unsigned char>(wparam);

		// If a key is pressed send it to the input object so it can record that state.
		//m_Input->KeyDown((unsigned int)wparam);
		return 0;
	}

	// Check if a key has been released on the keyboard.
	case WM_KEYUP:
	{
		unsigned char keycode = static_cast<unsigned char>(wparam);

		// If a key is released then send it to the input object so it can unset the state for that key.
		//m_Input->KeyUp((unsigned int)wparam);
		return 0;
	}
	case WM_CHAR:
	{
		unsigned char ch = static_cast<unsigned char>(wparam);

		return 0;
	}

	//Mouse Messages
	case WM_MOUSEMOVE:
	{
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		return 0;
	}
	case WM_RBUTTONDOWN:
	{
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		return 0;
	}
	case WM_MBUTTONDOWN:
	{
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		return 0;
	}
	case WM_LBUTTONUP:
	{
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		return 0;
	}
	case WM_RBUTTONUP:
	{
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		return 0;
	}
	case WM_MBUTTONUP:
	{
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		return 0;
	}
	case WM_MOUSEWHEEL:
	{
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		if (GET_WHEEL_DELTA_WPARAM(wparam) > 0)
		{
		}
		else if (GET_WHEEL_DELTA_WPARAM(wparam) < 0)
		{
		}
		return 0;
	}


	default:
	{
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}
	}
}
bool App::Initialize(HINSTANCE hInstance, int screenWidth, int screenHeight)
{
	bool result;
	result = _renderWindow.Initialize(this, screenWidth, screenHeight);

	if (!result)
	{
		return false;
	}

	result = this->_graphics.Initalize(this->_renderWindow.GetHWND(), screenWidth, screenHeight, 1, 100);

	if (!result)
	{
		return false;
	}

	return true;
}

bool App::ProcessMessages()
{
	return this->_renderWindow.ProcessMessages();
}

void App::Update()
{
	//TODO:: Read input event data, event managment
}
void App::RenderFrame()
{
	this->_graphics.Frame();

}
void App::Run()
{
	while (this->ProcessMessages())
	{
		this->Update();
		this->RenderFrame();
	}
}
void App::Shutdown()
{
	// Shutdown the window.

	this->_renderWindow.Shutdown();
}
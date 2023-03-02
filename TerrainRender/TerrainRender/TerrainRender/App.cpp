#include "App.h"
#include <windows.h>
#include "ErrorHandler.h"
#include <memory>
App::App()
{
	//https://learn.microsoft.com/en-us/windows/win32/inputdev/using-raw-input
	RAWINPUTDEVICE rid;
	rid.usUsagePage = 0x01;          // HID_USAGE_PAGE_GENERIC
	rid.usUsage = 0x02;              // HID_USAGE_GENERIC_MOUSE
	rid.dwFlags = 0;				
	rid.hwndTarget = 0;

	if ( RegisterRawInputDevices( &rid, 1, sizeof(rid) ) == FALSE )
	{
		ErrorHandler::log(GetLastError(), L"Failed to register raw input devices.");
		exit(-1);
	}
}
App::~App() {};

LRESULT CALLBACK App::WindowProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{

	//Keyboard Messages
	// clear keystate when window loses focus to prevent input getting "stuck"
	case WM_KILLFOCUS:
	{
		this->_keyboard.ClearCharBuffer();
		this->_keyboard.ClearKeyBuffer();
		return 0;
	}
	// Check if a key has been pressed on the keyboard.
	case WM_KEYDOWN:
	{
		unsigned char keycode = static_cast<unsigned char>(wparam);

		// If a key is pressed send it to the input object so it can record that state.
		if (!(lparam & 0x40000000) || this->_keyboard.IsAutoRepeatEnabled())
		{
			_keyboard.OnKeyPressed((keycode));
		}
		return 0;
	}

	// Check if a key has been released on the keyboard.
	case WM_KEYUP:
	{
		unsigned char keycode = static_cast<unsigned char>(wparam);

		// If a key is released then send it to the input object so it can unset the state for that key.
		_keyboard.OnKeyReleased((keycode));
		return 0;
	}
	case WM_CHAR:
	{
		unsigned char ch = static_cast<unsigned char>(wparam);
		_keyboard.OnChar((ch));
		return 0;
	}

	//Mouse Messages
	case WM_MOUSEMOVE:
	{
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		this->_mouse.OnMouseMove(x, y);
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		this->_mouse.OnLeftPressed(x, y);
		return 0;
	}
	case WM_RBUTTONDOWN:
	{
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		this->_mouse.OnRightPressed(x, y);
		return 0;
	}
	case WM_MBUTTONDOWN:
	{
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		this->_mouse.OnMiddlePressed(x, y);
		return 0;
	}
	case WM_LBUTTONUP:
	{
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		this->_mouse.OnLeftReleased(x, y);
		return 0;
	}
	case WM_RBUTTONUP:
	{
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		this->_mouse.OnRightReleased(x, y);
		return 0;
	}
	case WM_MBUTTONUP:
	{
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		this->_mouse.OnMiddleReleased(x, y);
		return 0;
	}
	case WM_MOUSEWHEEL:
	{
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		if (GET_WHEEL_DELTA_WPARAM(wparam) > 0)
		{
			this->_mouse.OnWheelUp(x, y);
		}
		else if (GET_WHEEL_DELTA_WPARAM(wparam) < 0)
		{
			this->_mouse.OnWheelDown(x, y);
		}
		return 0;
	}
	case WM_INPUT:
	{
		//https://learn.microsoft.com/en-us/windows/win32/inputdev/using-raw-input
		UINT dwSize;

		GetRawInputData((HRAWINPUT)lparam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
		if (dwSize > 0)
		{
			LPBYTE lpb = new BYTE[dwSize];
			if (lpb == NULL)
			{
				return DefWindowProc(hwnd, umessage, wparam, lparam);
			}

			if (GetRawInputData((HRAWINPUT)lparam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) == dwSize)
			{
				RAWINPUT* raw = (RAWINPUT*)lpb;
				if (raw->header.dwType == RIM_TYPEMOUSE)
				{
					if ((raw->data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE) == MOUSE_MOVE_ABSOLUTE)
					{
						//Note that if you want your game to support use via remote desktop (typically for testing), 
						//then you need to deal with emulating relative input from absolute position because in the case of a virtual desktop you never get relative,
						//only absolute, from WM_INPUT. This is indicated by MOUSE_VIRTUAL_DESKTOP.
						bool isVirtualDesktop = (raw->data.mouse.usFlags & MOUSE_VIRTUAL_DESKTOP) == MOUSE_VIRTUAL_DESKTOP;

						int width = GetSystemMetrics(isVirtualDesktop ? SM_CXVIRTUALSCREEN : SM_CXSCREEN);
						int height = GetSystemMetrics(isVirtualDesktop ? SM_CYVIRTUALSCREEN : SM_CYSCREEN);

						int absoluteX = int((raw->data.mouse.lLastX / 65535.0f) * width);
						int absoluteY = int((raw->data.mouse.lLastY / 65535.0f) * height);
						this->_mouse.OnMouseMoveRawAbsolute(absoluteX, absoluteY);
					}
					else if (raw->data.mouse.lLastX != 0 || raw->data.mouse.lLastY != 0)
					{
						int relativeX = raw->data.mouse.lLastX;
						int relativeY = raw->data.mouse.lLastY;
						this->_mouse.OnMouseMoveRawRelative(relativeX, relativeY);
					}
				}
			}
			else
			{
				OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));
			}

			delete[] lpb;
		}
		return DefWindowProc(hwnd, umessage, wparam, lparam);
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
	while (!this->_keyboard.KeyBufferIsEmpty())
	{
		KeyboardEvent e = this->_keyboard.ReadKey();

	}
	while (!this->_keyboard.CharBufferIsEmpty())
	{
		unsigned char c = this->_keyboard.ReadChar();
	}

	while (!this->_mouse.EventBufferIsEmpty())
	{
		MouseEvent e = this->_mouse.ReadEvent();

		if (e.GetType() == MouseEvent::RAW_MOVE_ABSOLUTE)
		{
			int x = e.GetPosX();
			int y = e.GetPosY();

		}
		else if (e.GetType() == MouseEvent::RAW_MOVE_RELATIVE)
		{
			int x = e.GetPosX();
			int y = e.GetPosY();

		}
	}
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
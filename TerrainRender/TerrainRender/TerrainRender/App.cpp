#include "App.h"
#include <windows.h>
#include "ErrorHandler.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
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
		Keyboard::GetInstance()->ClearCharBuffer();
		Keyboard::GetInstance()->ClearKeyBuffer();
		return 0;
	}
	// Check if a key has been pressed on the keyboard.
	case WM_KEYDOWN:
	{
		unsigned char keycode = static_cast<unsigned char>(wparam);

		// If a key is pressed send it to the input object so it can record that state.
		if (!(lparam & 0x40000000) || Keyboard::GetInstance()->IsAutoRepeatEnabled())
		{
			Keyboard::GetInstance()->OnKeyPressed((keycode));
		}
		return 0;
	}

	// Check if a key has been released on the keyboard.
	case WM_KEYUP:
	{
		unsigned char keycode = static_cast<unsigned char>(wparam);

		// If a key is released then send it to the input object so it can unset the state for that key.
		Keyboard::GetInstance()->OnKeyReleased((keycode));
		return 0;
	}
	case WM_CHAR:
	{
		unsigned char ch = static_cast<unsigned char>(wparam);
		Keyboard::GetInstance()->OnChar((ch));
		return 0;
	}

	//Mouse Messages
	case WM_MOUSEMOVE:
	{
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		Mouse::GetInstance()->OnMouseMove(x, y);
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		Mouse::GetInstance()->OnLeftPressed(x, y);
		return 0;
	}
	case WM_RBUTTONDOWN:
	{
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		Mouse::GetInstance()->OnRightPressed(x, y);
		return 0;
	}
	case WM_MBUTTONDOWN:
	{
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		Mouse::GetInstance()->OnMiddlePressed(x, y);
		return 0;
	}
	case WM_LBUTTONUP:
	{
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		Mouse::GetInstance()->OnLeftReleased(x, y);
		return 0;
	}
	case WM_RBUTTONUP:
	{
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		Mouse::GetInstance()->OnRightReleased(x, y);
		return 0;
	}
	case WM_MBUTTONUP:
	{
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		Mouse::GetInstance()->OnMiddleReleased(x, y);
		return 0;
	}
	case WM_MOUSEWHEEL:
	{
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		if (GET_WHEEL_DELTA_WPARAM(wparam) > 0)
		{
			Mouse::GetInstance()->OnWheelUp(x, y);
		}
		else if (GET_WHEEL_DELTA_WPARAM(wparam) < 0)
		{
			Mouse::GetInstance()->OnWheelDown(x, y);
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
						Mouse::GetInstance()->OnMouseMoveRawAbsolute(absoluteX, absoluteY);
					}
					else if (raw->data.mouse.lLastX != 0 || raw->data.mouse.lLastY != 0)
					{
						int relativeX = raw->data.mouse.lLastX;
						int relativeY = raw->data.mouse.lLastY;
						Mouse::GetInstance()->OnMouseMoveRawRelative(relativeX, relativeY);
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

	this->_model = new ModelLayer;
	this->_graphics = new Graphics;
	this->_dataAccess = new TextFileDataAccess;

	result = _model->Initalize(this->_dataAccess);
	if (!result)
	{
		return false;
	}

	this->_model->Attach(this->_graphics);

	result = _renderWindow.Initialize(this, screenWidth, screenHeight);

	if (!result)
	{
		return false;
	}

	result = _model->LoadTerrain(L"C:\\Users\\porostamasgabor\\Documents\\THESIS\\File\\Sphericon.stl");
	if (!result)
	{
		return false;
	}

	result = this->_graphics->Initalize(_model, this->_renderWindow.GetHWND(), (float)screenWidth, (float)screenHeight, 1, 100);

	if (!result)
	{
		return false;
	}

	_timer.Start();
	return true;
}

bool App::ProcessMessages()
{
	return this->_renderWindow.ProcessMessages();
}

void App::Update()
{
	float dt = (float)this->_timer.GetMilisecondsElapsed();
	this->_timer.Restart();
	while (!Keyboard::GetInstance()->KeyBufferIsEmpty())
	{
		KeyboardEvent e = Keyboard::GetInstance()->ReadKey();
		//this->ControlKeyboard(e, dt);

	}
	while (!Keyboard::GetInstance()->CharBufferIsEmpty())
	{
		unsigned char c = Keyboard::GetInstance()->ReadChar();
	}

	while (! Mouse::GetInstance()->EventBufferIsEmpty())
	{
		MouseEvent e = Mouse::GetInstance() -> ReadEvent();
		ControlMouse(e);
	}


	if (Keyboard::GetInstance()->KeyIsPressed('W'))
	{
		this->_graphics->_position.MoveForward(dt);
	}
	if (Keyboard::GetInstance()->KeyIsPressed('S'))
	{
		this->_graphics->_position.MoveBack(dt);
	}
	if (Keyboard::GetInstance()->KeyIsPressed('A'))
	{
		this->_graphics->_position.MoveLeft(dt);
	}
	if (Keyboard::GetInstance()->KeyIsPressed('D'))
	{
		this->_graphics->_position.MoveRight(dt);
	}




}

void App::ControlMouse(const MouseEvent& e)
{
	if (MouseEvent::Type::RAW_MOVE_ABSOLUTE == e.GetType())
	{
		static MouseEvent prev;
		if (prev.IsValid())
		{
			int yaw = e.GetPosX() - prev.GetPosX();
			int pitch = e.GetPosY() - prev.GetPosY();

			this->_graphics->_position.RotatePitchYaw((float)pitch,(float) yaw);

		}
		prev = e;
	}
	else if (MouseEvent::Type::RAW_MOVE_RELATIVE == e.GetType())
	{
		this->_graphics->_position.RotatePitchYaw((float)e.GetPosY(), (float)e.GetPosX());
	}
}

void App::ControlKeyboard(const KeyboardEvent& e, float dt)
{
	if (e.IsPress())
	{
		switch (e.GetKeyCode())
		{
		case 'W':
		{
			this->_graphics->_position.MoveForward(dt);
			break;
		}
		case 'A':
		{
			this->_graphics->_position.MoveLeft(dt);
			break;
		}
		case 'S':
		{
			this->_graphics->_position.MoveBack(dt);
			break;
		}
		case 'D':
		{
			this->_graphics->_position.MoveRight(dt);
			break;
		}
		default:
			break;
		}

	}
}
void App::RenderFrame()
{
	this->_graphics->Frame();

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
	this->_graphics->Shutdown();
	this->_model->Shutdown();
	Keyboard::Shutdown();
	Mouse::Shutdown();
	delete _graphics;
	delete _model;
	delete this->_dataAccess;
}
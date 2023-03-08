#include "App.h"
#include <windows.h>
#include "ErrorHandler.h"
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
	return _input.WindowProc(hwnd, umessage, wparam, lparam);
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

	result - _model->LoadTerrain(L"C:\\Users\\porostamasgabor\\Documents\\THESIS\\File\\Sphericon.stl");
	if (!result)
	{
		return false;
	}

	result = this->_graphics->Initalize(_model, this->_renderWindow.GetHWND(), screenWidth, screenHeight, 1, 100);

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
	float dt = this->_timer.GetMilisecondsElapsed();
	this->_timer.Restart();
	while (!this->_input._keyboard.KeyBufferIsEmpty())
	{
		KeyboardEvent e = this->_input._keyboard.ReadKey();
		//this->ControlKeyboard(e, dt);

	}
	while (!this->_input._keyboard.CharBufferIsEmpty())
	{
		unsigned char c = this->_input._keyboard.ReadChar();
	}

	while (!this->_input._mouse.EventBufferIsEmpty())
	{
		MouseEvent e = this->_input._mouse.ReadEvent();
		ControlMouse(e);
	}


	if (this->_input._keyboard.KeyIsPressed('W'))
	{
		this->_graphics->_position.MoveForward(dt);
	}
	if (this->_input._keyboard.KeyIsPressed('S'))
	{
		this->_graphics->_position.MoveBack(dt);
	}
	if (this->_input._keyboard.KeyIsPressed('A'))
	{
		this->_graphics->_position.MoveLeft(dt);
	}
	if (this->_input._keyboard.KeyIsPressed('D'))
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
	delete _graphics;
	delete _model;
	delete this->_dataAccess;
}
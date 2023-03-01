#include "App.h"
#include <windows.h>

App::App(){}
App::~App() {};

LRESULT CALLBACK App::WindowProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if a key has been pressed on the keyboard.
	case WM_KEYDOWN:
	{
		// If a key is pressed send it to the input object so it can record that state.
		//m_Input->KeyDown((unsigned int)wparam);
	return 0;
	}

	// Check if a key has been released on the keyboard.
	case WM_KEYUP:
	{
		// If a key is released then send it to the input object so it can unset the state for that key.
		//m_Input->KeyUp((unsigned int)wparam);
		return 0;
	}

	default:
	{
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}
	}
}
bool App::Initialize(HINSTANCE hInstance)
{
	bool result;
	result = _renderWindow.Initialize(this, 800,600);

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
void App::Run()
{
	while (this->ProcessMessages())
	{
		//this->Update();
		//this->RenderFrame();
	}
}
void App::Shutdown()
{
	// Shutdown the window.

	this->_renderWindow.Shutdown();
}
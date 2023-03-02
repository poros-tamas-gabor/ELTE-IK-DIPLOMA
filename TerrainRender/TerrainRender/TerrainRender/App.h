#ifndef APP_H
#define APP_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "RenderWindow.h"
#include "Graphics/Graphics.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Input/Input.h"
class App
{
private:
	RenderWindow	_renderWindow;
					
	Graphics		_graphics;
	Input			_input;

	void Update();
	void RenderFrame();
	bool ProcessMessages();

public:
	App();
	~App();

	bool Initialize(HINSTANCE hInstance, int screenWidth, int screenHeight);
	void Run();
	void Shutdown();

	LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

};
#endif
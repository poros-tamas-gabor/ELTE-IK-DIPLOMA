#ifndef APP_H
#define APP_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "RenderWindow.h"
class App
{
private:
	RenderWindow		_renderWindow;

	//GraphicsManager	_graphicsManager;
	//InputManager		_inputManager;


	void Update();
	void RenderFrame();
	bool ProcessMessages();

public:
	App();
	~App();

	bool Initialize(HINSTANCE hInstance);
	void Run();
	void Shutdown();

	LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

};
#endif
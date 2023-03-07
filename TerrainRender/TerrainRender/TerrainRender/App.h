#ifndef APP_H
#define APP_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "RenderWindow.h"
#include "Graphics/Graphics.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Input/Input.h"
#include "Timer.h"
#include "Model/ModelLayer.h"
#include <memory>
class App
{
private:
	RenderWindow					_renderWindow;
					
	Input							_input;
	Timer							_timer;
	std::unique_ptr<Graphics>		_graphics;
	std::shared_ptr<ModelLayer>		_model;

	void Update();
	void RenderFrame();
	bool ProcessMessages();
	void ControlKeyboard(const KeyboardEvent& e, float dt);
	void ControlMouse(const MouseEvent& e);

public:
	App();
	~App();

	bool Initialize(HINSTANCE hInstance, int screenWidth, int screenHeight);
	void Run();
	void Shutdown();

	LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

};
#endif
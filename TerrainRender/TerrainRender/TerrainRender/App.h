#ifndef APP_H
#define APP_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "RenderWindow.h"
#include "Graphics/Graphics.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Timer.h"
#include "Model/ModelLayer.h"
#include "Model/Persistence/DataAccess.h"
#include <memory>
#include "Controller/Controllers.h"
#include <vector>
class App
{
private:
	RenderWindow				_renderWindow;
					
	Timer						_timer;
	Graphics					_graphics;
	ModelLayer					_model;
	IDataAccess*				_dataAccess;
	std::vector<IController*>	_controllers;
	unsigned int				_currentController;

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
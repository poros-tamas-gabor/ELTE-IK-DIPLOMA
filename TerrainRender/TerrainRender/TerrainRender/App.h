#ifndef APP_H
#define APP_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "RenderWindow.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Timer.h"
#include "Model/Persistence/DataAccess.h"
#include <memory>
#include <vector>
#include "ImGui/imgui.h"
#include "Model/TerrainModel.h"
#include "Controller/CompositeController.h"
#include "View/TerrainView.h"

class App
{
private:

	RenderWindow				m_renderWindow;
	
	Timer						m_timer;
	TerrainModelPtr				m_terrainModel;
	TerrainViewPtr				m_terrainView;
	CompositeControllerPtr		m_terrainController;
	IDataAccessPtr				m_dataAccess;


	void Update();
	void RenderFrame();
	bool ProcessMessages();

public:
	App();
	~App();


	bool Initialize(HINSTANCE hInstance, int screenWidth, int screenHeight);
	void Run();
	void Shutdown();
	void Resize(UINT screenWidth, UINT screenHeight);
	MousePtr						m_mouse;
	KeyboardPtr						m_keyboard;

	LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

};
#endif
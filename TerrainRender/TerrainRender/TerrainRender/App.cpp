#include "App.h"
#include <windows.h>
#include "ErrorHandler.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "resource.h"

//TODO: DELETE
#include "Controller/GuiController.h"
#include "Controller/Controller3DExplore.h"
#include "Controller/ControllerFlythrough.h"


extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

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
		ErrorHandler::Log(GetLastError(), L"Failed to register raw input devices.");
		exit(-1);
	}

	this->m_terrainController = std::make_shared<CompositeController>();
	this->m_terrainModel = std::make_shared<TerrainModel>();
	this->m_terrainView = std::make_shared<TerrainView>();
	srand(time(nullptr));
}
App::~App() {};

LRESULT CALLBACK App::WindowProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, umessage, wparam, lparam))
		return true;

	ImGuiIO& io = ImGui::GetIO();
	switch (umessage)
	{

		//Keyboard Messages
		// clear keystate when window loses focus to prevent input getting "stuck"
	case WM_KILLFOCUS:
	{
		this->_keyboard.ClearCharBuffer();
		this->_keyboard.ClearKeyBuffer();
		this->_keyboard.ClearKeyStates();
		return 0;
	}

	case WM_SIZE:
	{
		UINT width = LOWORD(lparam);
		UINT height = HIWORD(lparam);
		//unsigned params[2] = {width, height};
		//this->m_terrainController->HandleMessage(WM_SIZE, NULL, params);
		Resize(width, height);
		return 0;
	}
	// Check if a key has been pressed on the keyboard.
	case WM_KEYDOWN:
	{
		if (io.WantCaptureKeyboard)
		{
			return 0;
		}
		unsigned char keycode = static_cast<unsigned char>(wparam);

		// If a key is pressed send it to the input object so it can record that state.
		if (!(lparam & 0x40000000) || this->_keyboard.IsAutoRepeatEnabled())
		{
			this->_keyboard.OnKeyPressed((keycode));
		}
		return 0;
	}

	// Check if a key has been released on the keyboard.
	case WM_KEYUP:
	{
		if (io.WantCaptureKeyboard)
		{
			return 0;
		}
		unsigned char keycode = static_cast<unsigned char>(wparam);

		// If a key is released then send it to the input object so it can unset the state for that key.
		this->_keyboard.OnKeyReleased((keycode));
		return 0;
	}
	case WM_CHAR:
	{
		if (io.WantCaptureKeyboard)
		{
			return 0;
		}
		unsigned char ch = static_cast<unsigned char>(wparam);
		this->_keyboard.OnChar((ch));
		return 0;
	}

	//Mouse Messages
	case WM_MOUSEMOVE:
	{
		if (io.WantCaptureMouse)
		{
			return 0;
		}
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		this->_mouse.OnMouseMove(x, y);
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		if (io.WantCaptureMouse)
		{
			return 0;
		}
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		this->_mouse.OnLeftPressed(x, y);
		return 0;
	}
	case WM_RBUTTONDOWN:
	{
		if (io.WantCaptureMouse)
		{
			return 0;
		}
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		this->_mouse.OnRightPressed(x, y);
		return 0;
	}
	case WM_MBUTTONDOWN:
	{
		if (io.WantCaptureMouse)
		{
			return 0;
		}
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		this->_mouse.OnMiddlePressed(x, y);
		return 0;
	}
	case WM_LBUTTONUP:
	{
		if (io.WantCaptureMouse)
		{
			return 0;
		}
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		this->_mouse.OnLeftReleased(x, y);
		return 0;
	}
	case WM_RBUTTONUP:
	{
		if (io.WantCaptureMouse)
		{
			return 0;
		}
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		this->_mouse.OnRightReleased(x, y);
		return 0;
	}
	case WM_MBUTTONUP:
	{
		if (io.WantCaptureMouse)
		{
			return 0;
		}
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		this->_mouse.OnMiddleReleased(x, y);
		return 0;
	}
	case WM_MOUSEWHEEL:
	{
		if (io.WantCaptureMouse)
		{
			return 0;
		}
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
		if (io.WantCaptureMouse)
		{
			return DefWindowProc(hwnd, umessage, wparam, lparam);
		}
		//https://learn.microsoft.com/en-us/windows/win32/inputdev/using-raw-input
		UINT dwSize = 0;

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
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	this->m_terrainView->SetController(this->m_terrainController);
	this->m_terrainView->SetModel(this->m_terrainModel);

	bool result;

	this->m_dataAccess = new BinaryFileDataAccessAsync;
	if (this->m_dataAccess == nullptr)
		return false;

	result = _renderWindow.Initialize(this, screenWidth, screenHeight);
	if (!result)
		return false;

	ImGui_ImplWin32_Init(this->_renderWindow.GetHWND());

	result = this->m_terrainController->Initialize(this->m_terrainModel, &this->_mouse, &this->_keyboard);
	if (!result)
		return false;

	//TODO : ne itt legyen
	{
		IControllerPtr c = std::make_shared<Controller3DExplore>();

		c->Initialize(m_terrainModel, &this->_mouse, &this->_keyboard);
		this->m_terrainController->AddController(c);

		IControllerPtr guic = std::make_shared<GuiController>();

		guic->Initialize(this->m_terrainModel, &this->_mouse, &this->_keyboard);
		this->m_terrainController->AddController(guic);

		IControllerPtr fly = std::make_shared<ControllerFlythrough>();

		fly->Initialize(this->m_terrainModel, &this->_mouse, &this->_keyboard);
		this->m_terrainController->AddController(fly);


	}

	result = this->m_terrainView->Initalize(_renderWindow.GetHWND(), (float)screenWidth, (float)screenHeight);
	if (!result)
		return false;

	this->m_terrainModel->m_modelMessageSystem.Subscribe(m_terrainView);

	result = this->m_terrainModel->Initalize(_renderWindow.GetHWND(), m_dataAccess, m_terrainView->GetDevice(), screenWidth, screenHeight, 1, 500);

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
	this->m_terrainController->HandleMessage(IDC_TIME_ELAPSED, &dt, NULL);
}


void App::RenderFrame()
{
	this->m_terrainView->RenderFrame();
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
	if (this->m_dataAccess)
	{
		delete this->m_dataAccess;
	}
	this->m_terrainController->Shutdown();
	this->m_terrainModel->Shutdown();
	this->m_terrainView->Shutdown();

	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	this->_renderWindow.Shutdown();
}

void App::Resize(UINT screenWidth, UINT screenHeight)
{
	m_terrainView->Resize(screenWidth, screenHeight);
	m_terrainModel->Resize(screenWidth, screenHeight);
}
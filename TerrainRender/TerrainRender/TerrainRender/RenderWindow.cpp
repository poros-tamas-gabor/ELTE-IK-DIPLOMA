#include "RenderWindow.h"
#include "ErrorHandler.h"
#include "App.h"
#include "resource.h"

static LRESULT CALLBACK MessageHandlerSetup(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
	case WM_NCCREATE:
	{
		AddMenus(hwnd);
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lparam);
		App* pApp = reinterpret_cast<App*>(pCreate->lpCreateParams);
		if (pApp == nullptr) //Sanity check
		{
			ErrorHandler::Log("Critical Error: Pointer to window container is null during WM_NCCREATE.");
			exit(-1);
		}
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pApp));
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(MessageHandlerRedirect));
		return pApp->WindowProc(hwnd, umessage, wparam, lparam);
	}
	default:
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}
}

static LRESULT CALLBACK MessageHandlerRedirect(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
	case WM_CLOSE:
	{
		DestroyWindow(hwnd);
		return 0;
	}
	default:
	{
		// retrieve ptr to window class
		App* const pApp = reinterpret_cast<App*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		// forward message to window class handler
		return pApp->WindowProc(hwnd, umessage, wparam, lparam);
	}
	}
}

static void AddMenus(HWND hwnd) {

	HMENU hMenubar;
	HMENU hMenuFile;
	HMENU hMenuFile_OpenTerrain;
	HMENU hMenuFile_OpenProject;
	HMENU hMenuHelp;
	HMENU hMenuWindows;

	hMenubar = CreateMenu();
	hMenuFile = CreateMenu();
	hMenuHelp = CreateMenu();
	hMenuFile_OpenTerrain = CreateMenu();
	hMenuFile_OpenProject = CreateMenu();
	hMenuWindows = CreateMenu();


	AppendMenuW(hMenuFile_OpenTerrain, MF_STRING, static_cast<UINT>(IDMENU_FIlE_TERRAIN_SHARP), L"&Terrain with Sharp edges");
	AppendMenuW(hMenuFile_OpenTerrain, MF_STRING, static_cast<UINT>(IDMENU_FIlE_TERRAIN_SOFT), L"&Terrain with Soft edges");

	AppendMenuW(hMenuFile, MF_POPUP, (UINT_PTR)hMenuFile_OpenTerrain, L"&Open Terrain");


	AppendMenuW(hMenuFile_OpenProject, MF_STRING, static_cast<UINT>(IDMENU_FIlE_TERRAIN_PROJECT_SHARP), L"&Project with Sharp edges");
	AppendMenuW(hMenuFile_OpenProject, MF_STRING, static_cast<UINT>(IDMENU_FIlE_TERRAIN_PROJECT_SOFT), L"&Project with Soft edges");

	AppendMenuW(hMenuFile, MF_POPUP, (UINT_PTR)hMenuFile_OpenProject, L"&Open Project");

	AppendMenuW(hMenuFile, MF_SEPARATOR, 0, NULL);
	AppendMenuW(hMenuFile, MF_STRING, static_cast<UINT>(IDMENU_FIlE_CAMERA_TRAJECTORY), L"&Open Trajectory");
	AppendMenuW(hMenuFile, MF_SEPARATOR, 0, NULL);
	AppendMenuW(hMenuFile, MF_STRING, static_cast<UINT>(IDMENU_FIlE_CONFIGURATION), L"&Open configure file");
	AppendMenuW(hMenuFile, MF_SEPARATOR, 0, NULL);

	AppendMenuW(hMenuFile, MF_STRING, static_cast<UINT>(IDMENU_FILE_OUTPUT_DIRECTORY), L"&Set output directory");


	AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenuFile, L"&File");


	AppendMenuW(hMenuWindows, MF_STRING, static_cast<UINT>(IDMENU_WINDOWS_GENERAL), L"&General settings");
	AppendMenuW(hMenuWindows, MF_STRING, static_cast<UINT>(IDMENU_WINDOWS_EXPLORE3D), L"&Explore3D settings");
	AppendMenuW(hMenuWindows, MF_STRING, static_cast<UINT>(IDMENU_WINDOWS_FLYTHROUGH), L"&Flythrough settings");

	AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenuWindows, L"&Settings");

	AppendMenuW(hMenubar, MF_STRING, static_cast<UINT>(IDMENU_HELP), L"&Help");

	SetMenu(hwnd, hMenubar);
}

bool RenderWindow::Initialize(App* pApp, int screenWidth, int screenHeight)
{
	int			posX, posY;
	RECT		wr;

	// Get the instance of this application.	
	this->m_hInstance = GetModuleHandle(NULL);

	this->RegisterWindowClass();

	posX = posY = 100;
	// calculate the size of the client area
	wr = { 0, 0, screenWidth, screenHeight };    // set the size, but not the position
	AdjustWindowRect(&wr, m_widnowStyle, TRUE);    // adjust the size

	this->m_width = wr.right - wr.left;
	this->m_height = wr.bottom - wr.top;

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(
							NULL,
							m_windowClass.c_str(),
							m_windowTitle.c_str(),
							m_widnowStyle,
							posX, posY, 
							this->m_width, this->m_height,
							NULL, NULL, m_hInstance, pApp);

	if (this->m_hwnd == nullptr)
	{
		ErrorHandler::Log(GetLastError(), L"CreateWindowEX Failed for window: " + m_windowTitle);
		return false;
	}
	// Bring the window up on the screen and set it as main focus.
	ShowWindow(this->m_hwnd, SW_SHOW);
	SetForegroundWindow(this->m_hwnd);
	SetFocus(this->m_hwnd);

	return true;
}
bool RenderWindow::ProcessMessages()
{
	MSG msg;
	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));


	// Handle the windows messages.
	while (PeekMessage(&msg, this->m_hwnd, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// Check if the window was closed
	if (msg.message == WM_NULL)
	{
		if (!IsWindow(this->m_hwnd))
		{
			this->m_hwnd = nullptr;
			UnregisterClass(this->m_windowClass.c_str(), this->m_hInstance);
			return false;
		}
	}

	return true;

}

HWND RenderWindow::GetHWND() const
{
	return this->m_hwnd;
}
void RenderWindow::Shutdown()
{
	if (this->m_hwnd != nullptr)
	{
		UnregisterClass(this->m_windowClass.c_str(), this->m_hInstance);
		DestroyWindow(this->m_hwnd);
	}


}

void RenderWindow::RegisterWindowClass()
{
	WNDCLASSEX	wc;

	// Setup the windows class with default settings.
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc		= MessageHandlerSetup;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= this->m_hInstance;
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm			= wc.hIcon;
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= m_windowClass.c_str();
	wc.cbSize			= sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);
}


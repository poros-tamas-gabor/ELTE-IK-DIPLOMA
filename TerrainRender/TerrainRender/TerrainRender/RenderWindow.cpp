#include "RenderWindow.h"
#include "ErrorHandler.h"
#include "App.h"

static LRESULT CALLBACK MessageHandlerSetup(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
	case WM_NCCREATE:
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lparam);
		App* pApp = reinterpret_cast<App*>(pCreate->lpCreateParams);
		if (pApp == nullptr) //Sanity check
		{
			ErrorHandler::log("Critical Error: Pointer to window container is null during WM_NCCREATE.");
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


bool RenderWindow::Initialize(App* pApp, int screenWidth, int screenHeight)
{
	int			posX, posY;
	RECT		wr;

	// Get the instance of this application.	
	this->_hInstance = GetModuleHandle(NULL);

	this->RegisterWindowClass();

	posX = posY = 100;
	// calculate the size of the client area
	wr = { 0, 0, screenWidth, screenHeight };    // set the size, but not the position
	AdjustWindowRect(&wr, _widnowStyle, FALSE);    // adjust the size

	this->_width = wr.right - wr.left;
	this->_height = wr.bottom - wr.top;

	// Create the window with the screen settings and get the handle to it.
	_hwnd = CreateWindowEx(
							NULL,
							_window_class.c_str(),
							_window_title.c_str(),
							_widnowStyle,
							posX, posY, 
							this->_width, this->_height,
							NULL, NULL, _hInstance, pApp);

	if (this->_hwnd == nullptr)
	{
		ErrorHandler::log(GetLastError(), L"CreateWindowEX Failed for window: " + _window_title);
		return false;
	}
	// Bring the window up on the screen and set it as main focus.
	ShowWindow(this->_hwnd, SW_SHOW);
	SetForegroundWindow(this->_hwnd);
	SetFocus(this->_hwnd);

	return true;
}
bool RenderWindow::ProcessMessages()
{
	MSG msg;
	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));


	// Handle the windows messages.
	while (PeekMessage(&msg, this->_hwnd, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// Check if the window was closed
	if (msg.message == WM_NULL)
	{
		if (!IsWindow(this->_hwnd))
		{
			this->_hwnd = nullptr;
			UnregisterClass(this->_window_class.c_str(), this->_hInstance);
			return false;
		}
	}

	return true;

}

HWND RenderWindow::GetHWND() const
{
	return this->_hwnd;
}
void RenderWindow::Shutdown()
{
	if (this->_hwnd != nullptr)
	{
		UnregisterClass(this->_window_class.c_str(), this->_hInstance);
		DestroyWindow(this->_hwnd);
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
	wc.hInstance		= this->_hInstance;
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm			= wc.hIcon;
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= _window_class.c_str();
	wc.cbSize			= sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);
}
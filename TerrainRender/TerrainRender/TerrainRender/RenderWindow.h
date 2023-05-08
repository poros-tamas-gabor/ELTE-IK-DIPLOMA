#ifndef RENDER_WINDOW_H
#define RENDER_WINDOW_H

///////////////////////////////////////////////////////////////////////////////
// RenderWindow.h
// ==============
//
// The RenderWindow is responsible to create a render window and setting up its message handler.
// The RenderWindow class contains methods to initialize, process messages, get the window handle, and shutdown the window.
// The AddMenus function creates menus for the window.
// 
// AUTHOR: Pindrought
// URL: https://github.com/Pindrought/DirectX-11-Engine-VS2017
// CREATED: Unknown
// MODIFIED: 2023-05-08 by TAMAS GABOR POROS
///////////////////////////////////////////////////////////////////////////////

#include "win.h"
#include <string>

class App;
class RenderWindow
{
private:
	HWND				m_hwnd = NULL; //Handle to this window
	HINSTANCE			m_hInstance = NULL; //Handle to application instance
	const std::wstring	m_windowTitle = L"Render Terrain"; //Wide string representation of window title
	const std::wstring	m_windowClass = L"Render Terrain Window Class"; //Wide string representation of window class name
	int					m_width = 0;
	int					m_height = 0;
	UINT				m_widnowStyle = WS_OVERLAPPEDWINDOW;

public:
	bool Initialize(App* app, int width, int height);
	bool ProcessMessages();
	HWND GetHWND() const;
	void Shutdown();

private:
	void RegisterWindowClass();
};

static void AddMenus(HWND hwnd);
static LRESULT CALLBACK MessageHandlerSetup (HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
static LRESULT CALLBACK MessageHandlerRedirect (HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

#endif
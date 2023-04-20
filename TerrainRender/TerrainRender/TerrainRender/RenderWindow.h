
#ifndef RENDER_WINDOW_H
#define RENDER_WINDOW_H

#include "win.h"
#include <string>

class App;
class RenderWindow
{
private:
	HWND				_hwnd = NULL; //Handle to this window
	HINSTANCE			_hInstance = NULL; //Handle to application instance
	const std::wstring	_window_title = L"Render Terrain"; //Wide string representation of window title
	const std::wstring	_window_class = L"Render Terrain Window Class"; //Wide string representation of window class name
	int					_width = 0;
	int					_height = 0;
	UINT				_widnowStyle = WS_OVERLAPPEDWINDOW;


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
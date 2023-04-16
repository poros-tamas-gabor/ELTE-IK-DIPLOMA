#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>


class ProgressBar
{
private:
	HWND				_hwndPB = NULL; //Handle to progress bar
	HWND				_hwndParent = NULL; //Handle to parent m_hwnd
	HINSTANCE			_hInstance = NULL; //Handle to application instance
	int					_width = 0;
	int					_height = 0;


public:
	bool Initialize(HINSTANCE hintance, HWND parentHWND);
	HWND GetHWND() const;
	void Shutdown();

};


#endif
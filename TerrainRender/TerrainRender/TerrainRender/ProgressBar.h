#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include "Model/Persistence/ICallable.h"
#include <thread>

class ProgressBar
{
private:
	HWND				_hwndPB = NULL; //Handle to progress bar
	HWND				_hwnd = NULL; //Handle to pop up
	HINSTANCE			_hInstance = NULL; //Handle to application instance
	int					_width = 0;
	int					_height = 0;
	bool&				_running;
	std::thread&		_worker;


public:
	ProgressBar(bool& running, std::thread& process);
	void Run();
	HWND GetHWND() const;
	void Shutdown();

};


#endif
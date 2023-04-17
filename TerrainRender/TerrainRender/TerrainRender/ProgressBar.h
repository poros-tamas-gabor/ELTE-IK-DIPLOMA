#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include "Model/Persistence/ICallable.h"
#include <atomic>

class ProgressBar
{
private:
	HWND				_hwndPB		= NULL; //Handle to progress bar
	HWND				_hwnd		= NULL; //Handle to pop up
	std::atomic_bool&	_running;


public:
	ProgressBar(std::atomic_bool& running);
	void Run();
};


#endif
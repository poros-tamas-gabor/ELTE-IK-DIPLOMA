#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H

///////////////////////////////////////////////////////////////////////////////
// ProgessBar.h
// ============
//
// The ProgressBar class provides a way to create a pop-up window with a marquee progress bar using Win32 API. 
// The constructor takes in an std::atomic_bool reference that indicates whether the process associated with the progress bar is still running or has completed. 
// The Run function creates the progress bar window and sets its range, position, and marquee options. 
// Once the process is complete, the progress bar window is destroyed.
//
// AUTHOR: TAMAS GABOR POROS
// CREATED: 2023-05-08
///////////////////////////////////////////////////////////////////////////////

#include "win.h"
#include <string>
#include "Model/Persistence/ICallable.h"
#include <atomic>

class ProgressBar
{
private:
	HWND				m_hwndPB	= NULL; //Handle to progress bar
	HWND				m_hwnd		= NULL; //Handle to pop up
	std::atomic_bool&	m_running;

public:
	ProgressBar(std::atomic_bool& running);
	void Run();
};
#endif
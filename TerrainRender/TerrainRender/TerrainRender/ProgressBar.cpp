#include "ProgressBar.h"
#include <CommCtrl.h>
#include "ErrorHandler.h"
#include "resource.h"
#include <thread>
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")


ProgressBar::ProgressBar(std::atomic_bool& running) : _running(running)
{
	_hwnd = CreateWindowEx(0, WC_DIALOG, L"Proccessing...", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		600,300, 280, 120, NULL, NULL, NULL, NULL);

	_hwndPB = CreateWindowEx(NULL, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE | PBS_MARQUEE, 40, 20, 200, 20,
		_hwnd, (HMENU)1, NULL, NULL);
}



void ProgressBar::Run()
{
	MSG msg;

	PostMessage(_hwndPB, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
	PostMessage(_hwndPB, PBM_SETPOS, 0, 0);
	PostMessage(_hwndPB, PBM_SETMARQUEE, (WPARAM)TRUE, (LPARAM)0);
	while ( GetMessage(&msg, NULL, NULL, NULL))
	{
		if (!_running || msg.message == WM_CLOSE)
		{
			DestroyWindow(_hwnd);
			PostMessage(_hwndPB, PBM_SETMARQUEE, (WPARAM)FALSE, (LPARAM)0);
			return;
		}
	}
}




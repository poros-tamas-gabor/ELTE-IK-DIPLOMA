#include "Tasks.h"


#include <CommCtrl.h>
#include "../ErrorHandler.h"
#include "../resource.h"
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")


Task_LoadTerrainSoft::Task_LoadTerrainSoft(const std::wstring& filepath, IModelPtr modelPtr) : m_filepath(filepath), m_modelPtr(modelPtr) {}
void Task_LoadTerrainSoft::Task()
{
	m_modelPtr->LoadTerrainSoftEdges(m_filepath.c_str());
}
void Task_LoadTerrainSoft::operator()()
{
	Task();
 }

Task_LoadingWindow::Task_LoadingWindow(std::atomic_bool& keepRunning) : m_keepRunning(keepRunning)
{
	//PostMessage(m_hwnd, WM_INITDIALOG, 0, 0);
	m_hwnd = CreateWindowEx(0, WC_DIALOG, L"Proccessing...", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		600, 300, 280, 120, NULL, NULL, NULL, NULL);
	//m_hwndPB = CreateWindowEx(NULL, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE | PBS_MARQUEE, 40, 20, 200, 20,
	//	m_hwnd, (HMENU)1000, NULL, NULL);

}

HWND Task_LoadingWindow::GetHWND() const
{
	return m_hwnd;
}

void Task_LoadingWindow::Task()
{
	MSG msg;

    SendMessage(m_hwndPB, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
    SendMessage(m_hwndPB, PBM_SETPOS, 0, 0);
    SendMessage(m_hwndPB, PBM_SETMARQUEE, (WPARAM)TRUE, (LPARAM)0);

    while (m_keepRunning)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    PostMessage(m_hwndPB, PBM_SETMARQUEE, (WPARAM)FALSE, (LPARAM)0);
    PostMessage(m_hwnd, WM_CLOSE, 0, 0);
    if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    DestroyWindow(m_hwnd);
}

void Task_LoadingWindow::operator()()
{
	Task();


}

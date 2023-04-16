#include "ProgressBar.h"
#include <CommCtrl.h>
#include "ErrorHandler.h"
#include "resource.h"
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

bool ProgressBar::Initialize(HINSTANCE hintance, HWND parentHWND)
{
	//https://learn.microsoft.com/en-us/windows/win32/controls/create-progress-bar-controls
	RECT rcClient;  // Client area of parent window.
	int cyVScroll;  // Height of scroll bar arrow.
	HANDLE hFile;   // Handle of file.
	DWORD cb;       // Size of file and count of bytes read.
	LPCH pch;       // Address of data read from file.
	LPCH pchTmp;    // Temporary pointer.

	_hwndParent = parentHWND;
	_hInstance = hintance;
	//InitCommonControls();

	GetClientRect(_hwndParent, &rcClient);
	cyVScroll = GetSystemMetrics(SM_CYVSCROLL);


	_hwndPB = CreateWindowEx(0, PROGRESS_CLASS, (LPTSTR)NULL,
		WS_CHILD | WS_VISIBLE,
		(rcClient.right - 200) / 2, (rcClient.bottom - rcClient.top) / 2 - 5,
		200, 20,
		
		
		//rcClient.left,
		//rcClient.bottom - cyVScroll,
		//rcClient.right, cyVScroll,
		_hwndParent, (HMENU)ID_PROGRESS_BAR, _hInstance, NULL);



	if (this->_hwndPB == nullptr)
	{
		ErrorHandler::Log(GetLastError(), L"CreateWindowEX Failed for loading bar" );
		return false;
	}
	return true;
}
HWND ProgressBar::GetHWND() const
{
	return _hwndPB;
}


void ProgressBar::Shutdown()
{
	if (this->_hwndPB != nullptr)
	{
		DestroyWindow(this->_hwndPB);
	}
}





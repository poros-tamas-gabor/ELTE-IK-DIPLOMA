#include "DialogWin.h"
#include <commdlg.h>
#include <shlobj_core.h>

void OpenFileDialog(std::wstring& filePathstr, const wchar_t* filter)
{
	wchar_t filePath[260];      // buffer for file name
	OPENFILENAME ofn;			// common dialog box structure
	HWND hwnd = NULL;           // owner window

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = filePath;
	//
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	//
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(wchar_t) * 260;
	ofn.lpstrFilter = filter;//fileFilter.c_str();
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box. 
	if (GetOpenFileName(&ofn) == TRUE)
	{
		filePathstr = filePath;
	}
}

void OpenFileDialogDirectory(std::wstring& directory)
{
	BROWSEINFO bi = { 0 };
	bi.lpszTitle = L"Select a folder";
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	if (pidl != 0)
	{
		// get the name of the folder
		TCHAR path[MAX_PATH];
		if (SHGetPathFromIDList(pidl, path))
		{
			directory = path;
		}
		// free memory used
		IMalloc* imalloc = 0;
		if (SUCCEEDED(SHGetMalloc(&imalloc)))
		{
			imalloc->Free(pidl);
			imalloc->Release();
		}
	}
}

void OpenFileDialogMultipleSelection(std::vector<std::wstring>& files, const wchar_t* filter)
{
	wchar_t filePath[260];      // buffer for file name
	OPENFILENAME ofn;			// common dialog box structure
	HWND hwnd = NULL;           // owner window

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = filePath;
	//
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	//
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(wchar_t) * 260;
	ofn.lpstrFilter = filter;//L"All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR | OFN_ALLOWMULTISELECT | OFN_EXPLORER;

	// Display the Open dialog box. 
	if (GetOpenFileName(&ofn) == TRUE)
	{
		wchar_t* str = ofn.lpstrFile;
		std::wstring directory = str;
		str += (directory.length() + 1);

		//Only one element was selected
		if (*str == '\0')
			files.push_back(directory);
		while (*str) {
			std::wstring filename = str;
			str += (filename.length() + 1);
			files.push_back(directory + L"\\" + filename);
		}
	}
}
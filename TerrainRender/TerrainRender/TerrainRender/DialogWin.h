#ifndef DIALOG_WIN_H
#define DIALOG_WIN_H

///////////////////////////////////////////////////////////////////////////////
// DialogWin.h
// ===========
//
// The "DialogWin.h" header file provides three functions for creating a dialog window using the Win32 API to open files or directories.
//
// The functions included are:
// - "OpenFileDialog" which opens a dialog window for selecting a single file, based on the specified file filter.
// - "OpenFileDialogDirectory" which opens a dialog window for selecting a directory.
// - "OpenFileDialogMultipleSelection" which opens a dialog window for selecting multiple files, based on the specified file filter.
//
// Each function takes in parameters for storing the user's selection, such as the file path or a vector of file paths.
//
// AUTHOR: TAMAS GABOR POROS
// CREATED: 2023-05-08
///////////////////////////////////////////////////////////////////////////////

#include "win.h"
#include <string>
#include <vector>

//The function opens a dialog window for selecting a single file, based on the specified file filter.
void OpenFileDialog(std::wstring& filePath, const wchar_t* filter);
//The function opens a dialog window for selecting a directory.
void OpenFileDialogDirectory(std::wstring& directory);
//The function opens a dialog window for selecting multiple files, based on the specified file filter
void OpenFileDialogMultipleSelection(std::vector<std::wstring>& files, const wchar_t* filter);

#endif // !DIALOG_WIN_H
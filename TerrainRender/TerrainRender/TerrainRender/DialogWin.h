#ifndef DIALOG_WIN_H
#define DIALOG_WIN_H

#include "win.h"
#include <string>
#include <vector>

void OpenFileDialog(wchar_t* filePath, unsigned buffer, const wchar_t* filter);
void OpenFileDialogDirectory(std::wstring& directory);
void OpenFileDialogMultipleSelection(std::vector<std::wstring>& files, const wchar_t* filter);

#endif // !COM_EXCEPTION_H
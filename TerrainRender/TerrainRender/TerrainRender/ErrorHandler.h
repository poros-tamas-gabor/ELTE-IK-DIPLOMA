#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <comdef.h>
#include <string>

class ErrorHandler
{
public:
	static void log(std::string message);
	static void log(HRESULT hr, std::wstring message);
};
#endif
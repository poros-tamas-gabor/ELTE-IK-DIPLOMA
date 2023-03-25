#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <comdef.h>
#include <string>
#include "COMException.h"
#include "TRException.h"

class ErrorHandler
{
public:
	static void Log(std::string message);
	static void Log(HRESULT hr, std::wstring message);
	static void Log(const COMException& exception);
	static void Log(const TRException& exception);
	static void Log(const std::exception& exception);
};
#endif
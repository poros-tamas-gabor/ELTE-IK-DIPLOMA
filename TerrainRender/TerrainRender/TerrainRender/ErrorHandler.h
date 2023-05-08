#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

///////////////////////////////////////////////////////////////////////////////
// ErrorHandler.h
// ==============
//
// The `ErrorHandler` class provides a set of functions for displaying error messages in a Windows application via Win32 API.
// It includes several overloaded `Log` functions to handle different types of exceptions or error messages. 
// The error messages are displayed using the `MessageBox` function, which creates a popup message box with the specified error message and an error icon.
// The class is intended to be used as a centralized location for handling errors throughout an application.
//
// AUTHOR: Pindrought
// URL: https://github.com/Pindrought/DirectX-11-Engine-VS2017
// CREATED: Unknown
// MODIFIED: 2023-05-08 by TAMAS GABOR POROS
///////////////////////////////////////////////////////////////////////////////

#include "win.h"
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
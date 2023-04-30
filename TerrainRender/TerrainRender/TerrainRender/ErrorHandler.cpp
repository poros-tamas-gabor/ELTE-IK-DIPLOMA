#include "ErrorHandler.h"

void ErrorHandler::Log(std::string message)
{
	std::string error_message = "Error: " + message;
#ifndef _GTEST
	MessageBoxA(NULL, error_message.c_str(), "Error", MB_ICONERROR);
#endif // !_GTEST

}

void ErrorHandler::Log(HRESULT hr, std::wstring message)
{
	_com_error error(hr);
	std::wstring error_message = L"Error: " + message + L"\n" + error.ErrorMessage();
#ifndef _GTEST
	MessageBoxW(NULL, error_message.c_str(), L"Error", MB_ICONERROR);
#endif // !_GTEST
}

void ErrorHandler::Log(const COMException& exception)
{
	std::wstring error_message = exception.what();
#ifndef _GTEST
	MessageBoxW(NULL, error_message.c_str(), L"Error", MB_ICONERROR);
#endif // !_GTEST
}

void ErrorHandler::Log(const TRException& exception)
{
	std::wstring error_message = exception.what();
#ifndef _GTEST
	MessageBoxW(NULL, error_message.c_str(), L"Error", MB_ICONERROR);
#endif // !_GTEST
}

void ErrorHandler::Log(const std::exception& exception)
{
	std::string error_message = exception.what();
#ifndef _GTEST
	MessageBoxA(NULL, error_message.c_str(), "Error", MB_ICONERROR);
#endif // !_GTEST
}


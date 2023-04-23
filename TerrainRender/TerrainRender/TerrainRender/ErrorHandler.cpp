#include "ErrorHandler.h"

void ErrorHandler::Log(std::string message)
{
	std::string error_message = "Error: " + message;
	MessageBoxA(NULL, error_message.c_str(), "Error", MB_ICONERROR);
}

void ErrorHandler::Log(HRESULT hr, std::wstring message)
{
	_com_error error(hr);
	std::wstring error_message = L"Error: " + message + L"\n" + error.ErrorMessage();
	MessageBoxW(NULL, error_message.c_str(), L"Error", MB_ICONERROR);
}

void ErrorHandler::Log(const COMException& exception)
{
	std::wstring errorMsg = exception.what();
	MessageBoxW(NULL, errorMsg.c_str(), L"Error", MB_ICONERROR);
}

void ErrorHandler::Log(const TRException& exception)
{
	std::wstring errorMsg = exception.what();
	MessageBoxW(NULL, errorMsg.c_str(), L"Error", MB_ICONERROR);
}

void ErrorHandler::Log(const std::exception& exception)
{
	std::string errorMsg = exception.what();
	MessageBoxA(NULL, errorMsg.c_str(), "Error", MB_ICONERROR);
}


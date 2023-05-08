#ifndef COM_EXCEPTION_H
#define COM_EXCEPTION_H

///////////////////////////////////////////////////////////////////////////////
// COMException.h
// ==============
// 
// This header file contains the declaration of the COMException class which is designed to handle exceptions thrown from Microsoft Component Object Model (COM) APIs. 
// The class has a constructor that takes in the HRESULT value of the exception, a message string, and the file, function, and line number where the exception occurred. 
// The class also overrides the what() method to return the concatenated error message as a wide character string. 
// 
// Additionally, there is a preprocessor macro defined to make it easier to throw a COMException if the HRESULT of an API call indicates failure.
//
// AUTHOR: Pindrought
// URL: https://github.com/Pindrought/DirectX-11-Engine-VS2017
// CREATED: Unknown
// MODIFIED: 2023-05-08 by TAMAS GABOR POROS
///////////////////////////////////////////////////////////////////////////////

#include <comdef.h>
#include "StringConverter.h"
#include <string>

#define THROW_COM_EXCEPTION_IF_FAILED(hr, msg) if (FAILED (hr)) throw COMException( hr, msg, __FILEW__, __FUNCTIONW__, __LINE__ )

class COMException
{
private:
	std::wstring m_whatmsg;
public:

	COMException(HRESULT hr, const std::wstring& msg, const std::wstring& file, const std::wstring& function, int line)
	{
		_com_error error(hr);
		m_whatmsg = L"Msg: " + msg + L"\n";
		m_whatmsg += error.ErrorMessage();
#ifdef _DEBUG
		m_whatmsg += L"\nFile: " + file;
		m_whatmsg += L"\nFunction: " + function;
		m_whatmsg += L"\nLine: " + std::to_wstring(line);
#endif // _DEBUG
	}

	const wchar_t* what() const
	{
		return m_whatmsg.c_str();
	}
};
#endif // !COM_EXCEPTION_H
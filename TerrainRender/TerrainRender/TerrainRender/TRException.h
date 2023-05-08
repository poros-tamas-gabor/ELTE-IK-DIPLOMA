#ifndef TR_EXCEPTION_H
#define TR_EXCEPTION_H

///////////////////////////////////////////////////////////////////////////////
// TRException.h
// ==============
//
// This header file contains the declaration of the TRException class, which is designed to handle exceptions thrown from the Terrain Render application.
// The class has a constructor that takes a message string, the file, function, and line number where the exception occurred.
// The class also overrides the what() method to return the concatenated error message as a wide character string.
//
// Additionally, there is a preprocessor macro defined to make it easier to throw a TRException if the isCorrect bool type is false.
//
// AUTHOR: TAMAS GABOR POROS
// CREATED: 2023-05-08
///////////////////////////////////////////////////////////////////////////////

#include <comdef.h>
#include "StringConverter.h"
#include <string>

#define THROW_TREXCEPTION(msg) throw TRException(msg,__FILE__, __FUNCTION__, __LINE__ )
#define THROW_TREXCEPTION_IF_FAILED(isCorrect, msg) if(!isCorrect) throw TRException(msg,__FILE__, __FUNCTION__, __LINE__ )
class TRException 
{
private:
	std::wstring m_whatmsg;
public:
	TRException(const std::wstring& msg, const std::string& file, const std::string& function, int line)
	{
		m_whatmsg = L"Message: " + msg + L"\n";
#ifdef _DEBUG
		m_whatmsg += L"\nFile: " + StringConverter::StringToWide(file);
		m_whatmsg += L"\nFunction: " + StringConverter::StringToWide(function);
		m_whatmsg += L"\nLine: " + StringConverter::StringToWide(std::to_string(line));
#endif // _DEBUG
	}

	TRException(const std::wstring& msg, const std::wstring& file, const std::wstring& function, int line)
	{
		m_whatmsg = L"Message: " + msg + L"\n";
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
#endif // !TR_EXCEPTION_H
#ifndef TR_EXCEPTION_H
#define TR_EXCEPTION_H

#include <comdef.h>
#include "StringConverter.h"
#include <string>

#define THROW_TREXCEPTION(msg) throw TRException(msg,__FILE__, __FUNCTION__, __LINE__ )
class TRException 
{
private:
	std::wstring m_whatmsg;
public:
	TRException(const std::wstring& msg, const std::string& file, const std::string& function, int line)
	{
		m_whatmsg = L"Message: " + msg + L"\n";
		m_whatmsg += L"\nFile: " + StringConverter::StringToWide(file);
		m_whatmsg += L"\nFunction: " + StringConverter::StringToWide(function);
		m_whatmsg += L"\nLine: " + StringConverter::StringToWide(std::to_string(line));
	}

	TRException(const std::wstring& msg, const std::wstring& file, const std::wstring& function, int line)
	{
		m_whatmsg = L"Message: " + msg + L"\n";
		m_whatmsg += L"\nFile: " + file;
		m_whatmsg += L"\nFunction: " + function;
		m_whatmsg += L"\nLine: " + std::to_wstring(line);
	}

	const wchar_t* what() const
	{
		return m_whatmsg.c_str();
	}
};
#endif // !TR_EXCEPTION_H
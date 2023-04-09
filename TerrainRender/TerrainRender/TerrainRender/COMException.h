#ifndef COM_EXCEPTION_H
#define COM_EXCEPTION_H

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
		m_whatmsg += L"\nFile: " + file;
		m_whatmsg += L"\nFunction: " + function;
		m_whatmsg += L"\nLine: " + std::to_wstring(line);
	}

	const wchar_t* what() const
	{
		return m_whatmsg.c_str();
	}
};
#endif // !COM_EXCEPTION_H
#include "StringConverter.h"
#include <locale>
#include <codecvt>
std::wstring StringConverter::StringToWide(const std::string& str)
{
	std::wstring wstr;
	size_t size;
	wstr.resize(str.length());
	mbstowcs_s(&size, &wstr[0], wstr.size() + 1, str.c_str(), str.size());
	return wstr;
}

std::string StringConverter::WideToString(const std::wstring& wstr)
{
	std::string str;
	size_t size;
	str.resize(wstr.length());
	wcstombs_s(&size, &str[0], str.size() + 1, wstr.c_str(), wstr.size());
	return str;
}

std::wstring StringConverter::GetFileNameFromPath(const std::wstring& path)
{
	size_t pos = path.find_last_of(L"\\/");
	if (pos != std::wstring::npos)
	{
		return path.substr(pos + 1);
	}
	return path;
}
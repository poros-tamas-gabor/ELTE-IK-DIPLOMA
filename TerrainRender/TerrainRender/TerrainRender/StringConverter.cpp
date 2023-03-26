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
	//throw std::exception("stringtowide");
	//using convert_typeWS = std::codecvt_utf8<wchar_t>;
	//std::wstring_convert<convert_typeWS, wchar_t> converterWS;
	//
	//return converterWS.from_bytes(str);
}

std::string StringConverter::WideToString(const std::wstring& wstr)
{
	std::string str;
	size_t size;
	str.resize(wstr.length());
	wcstombs_s(&size, &str[0], str.size() + 1, wstr.c_str(), wstr.size());
	return str;
	////throw std::exception("stringtowide");
	//using convert_typeS = std::codecvt_utf8<wchar_t>;
	//std::wstring_convert<convert_typeS, wchar_t> converterS;
	//
	//return converterS.to_bytes(wstr);
}
#include "StringConverter.h"
#include <locale>
#include <codecvt>
std::wstring StringConverter::StringToWide(const std::string& str)
{
	throw std::exception("stringtowide");
	//using convert_typeWS = std::codecvt_utf8<wchar_t>;
	//std::wstring_convert<convert_typeWS, wchar_t> converterWS;
	//
	//return converterWS.from_bytes(str);
}

std::string StringConverter::WideToString(const std::wstring& wstr)
{
	throw std::exception("stringtowide");
	//using convert_typeS = std::codecvt_utf8<wchar_t>;
	//std::wstring_convert<convert_typeS, wchar_t> converterS;
	//
	//return converterS.to_bytes(wstr);
}
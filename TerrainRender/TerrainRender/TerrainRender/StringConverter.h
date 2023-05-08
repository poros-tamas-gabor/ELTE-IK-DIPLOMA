#ifndef STRING_CONVERTER_H
#define STRING_CONVERTER_H
///////////////////////////////////////////////////////////////////////////////
// StringConverter.h
// =================
// 
// The StringConverter class provides a set of static methods for converting and manipulating strings of different types and formats.
//
// AUTHOR: Pindrought
// URL: https://github.com/Pindrought/DirectX-11-Engine-VS2017
// CREATED: Unknown
// MODIFIED: 2023-05-08 by TAMAS GABOR POROS
///////////////////////////////////////////////////////////////////////////////
#include <string>

class StringConverter
{
public:
	static std::wstring StringToWide(const std::string& str);
	static std::string WideToString(const std::wstring& str);
	static std::wstring GetFileNameFromPath(const std::wstring& path);
};
#endif

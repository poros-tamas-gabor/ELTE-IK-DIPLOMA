#ifndef STRING_CONVERTER_H
#define STRING_CONVERTER_H

#include <string>
class StringConverter
{
public:
	static std::wstring StringToWide(const std::string& str);
	static std::string WideToString(const std::wstring& str);
};
#endif

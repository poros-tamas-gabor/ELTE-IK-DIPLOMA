#ifndef STRING_CONVERTER_H
#define STRING_CONVERTER_H

#include <string>
class StringConverter
{
public:
	static std::wstring StringToWide(const std::string& str);
	static std::string WideToString(const std::wstring& str);
	static std::wstring GetFileNameFromPath(const std::wstring& path)
	{
		size_t pos = path.find_last_of(L"\\/");
		if (pos != std::wstring::npos)
		{
			return path.substr(pos + 1);
		}
		return path;
	}
};
#endif

#include "General.h"
#include<map>
bool encrypt::General::areEqual(const std::wstring& str1, const std::wstring& str2) noexcept
{
	return str1 == str2;
}

//===========================================================================================

std::map<std::wstring, std::wstring> encrypt::General::buildParams(wchar_t** argv, size_t argc) noexcept
{
	std::map<std::wstring, std::wstring> params;
	for (int i = 3; i < argc; i++) {
		if (argv[i][0] == L'-' && i + 1 < argc && argv[i + 1][0] != L'-')
		{
			// параметр со значением
			params[argv[i]] = argv[i + 1];
		} else 
		{
			// параметр без значения, например флаг
			params[argv[i]] = L"";
		}
	}
	return params;
}

std::map<std::wstring, std::wstring> encrypt::General::buildParams(std::wistream& wcin, std::vector<std::wstring>& vec) noexcept
{
	std::map<std::wstring, std::wstring> params;
	std::wstring line;
	std::getline(wcin, line);

	for (int i{0}; i < line.length(); ++i) {
		int pos = line.find(' ', i);
		if (pos < 0) { vec.push_back(line.substr(i) ); break; }
		int count = pos - i;
		vec.push_back(line.substr(i, count));
		i = pos;
	}
	size_t argc = vec.size();

	for (size_t i = 0; i < argc;++i) {
		if (vec[i][0] == L'-' && i + 1 < argc && vec[i + 1][0] != L'-')
		{
			// параметр со значением
			params[vec[i]] = vec[i + 1];
		} else
		{
			// параметр без значения, например флаг
			params[vec[i]] = L"";
		}
	}

	return params;
}
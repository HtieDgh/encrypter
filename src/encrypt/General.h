#pragma once
#include<string>
#include<sstream>
#include"InputStrategy.h"
#include"EncrypterConfig.h"

namespace encrypt {
	class General
	{
	public:
		//Сравнивает строки
		static bool areEqual(const std::wstring& str1, const std::wstring& str2) noexcept;
		
		//Подготовка параметров алгоритмов к обработке.
		static std::map<std::wstring, std::wstring> buildParams(wchar_t** argv, size_t argc) noexcept;
		//работает только с std::wcin. vec - массив токенов
		static std::map<std::wstring, std::wstring> buildParams(std::wistream& wcin, std::vector<std::wstring>& vec) noexcept;
	};
}
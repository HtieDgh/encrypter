/*
   Copyright 2026 Htie digital

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

	   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#include "InputStrategy.h"
#include "Translator.h"
using T = encrypt::Translator;
//======================================================================
long long encrypt::StdInput::read(char* data,std::streamsize size)
{
	return std::cin.read(data,size), std::cin.gcount();
}
long long encrypt::StdInput::read(wchar_t* data, std::streamsize size)
{
	return std::wcin.read(data, size), std::wcin.gcount();
}
long long encrypt::StdInput::readln(std::string& data, char delim)
{
	return std::getline(std::cin, data, delim), data.length();
}
long long encrypt::StdInput::readln(std::wstring& data, wchar_t delim)
{
	return std::getline(std::wcin, data, delim), data.length();
}
void encrypt::StdInput::reset()
{
	std::wcin.clear();
	std::wcin.seekg(0, std::ios::beg);
	std::cin.clear();
	std::cin.seekg(0, std::ios::beg);
}
//==============================================================
encrypt::FileInput::FileInput(const char* path, int openType)
{
	this->fin.open(path, openType);
	if (!this->fin.is_open()) {
		throw T::msg({L"iostrategy",1});
	}
}

encrypt::FileInput::FileInput(std::wstring path, int openType)
{
	this->wfin.open(path, openType);
	if (!this->wfin.is_open()) {
		throw T::msg({L"iostrategy",1});
	}
	// Установка "парсера" битов в соответствии с utf-8. По умолначнию считывает по 1 байту в каждый wchar - что бред, 
	// теперь считывает по 2 байта - что правильно
	this->wfin.imbue(std::locale(".UTF-8"));//попытка исправить чтение с однобайтовго режима 
}

long long encrypt::FileInput::read(char* data, std::streamsize size)
{
	if (!this->fin.is_open()) {
		throw T::msg({L"iostrategy",2});
	}
	return this->fin.read(data, size), this->fin.gcount();
}

long long encrypt::FileInput::read(wchar_t* data, std::streamsize size)
{
	if (!this->wfin.is_open()) {
		throw T::msg({L"iostrategy",2});
	}
	return this->wfin.read(data, size), this->wfin.gcount();
}

long long encrypt::FileInput::readln(std::string& data, char delim)
{
	if (!this->fin.is_open()) {
		throw T::msg({L"iostrategy",2});
	}
	if (std::getline(this->fin, data, delim)) {
		return data.length();
	} else {
		return 0;
	}
}

long long encrypt::FileInput::readln(std::wstring& data, wchar_t delim)
{
	if (!this->wfin.is_open()) {
		throw T::msg({L"iostrategy",2});
	}
	if (std::getline(this->wfin, data, delim)) {
		return data.length();
	} else {
		return 0;
	}
}
long long encrypt::StdInput::operator>>(std::wstring& data)
{
	return std::wcin>>data, data.length();
}
long long encrypt::StdInput::operator>>(std::string& data)
{
	return std::cin >> data, data.length();
}
void encrypt::FileInput::reset() {
	if (this->wfin.is_open()) {
		wfin.clear();
		wfin.seekg(0, std::ios::beg);
	}
	if (this->fin.is_open()) {
		fin.clear();
		fin.seekg(0, std::ios::beg);
	}
}

long long encrypt::FileInput::operator>>(std::wstring& data)
{
	if (this->wfin.is_open()) {
		return this->wfin >> data, data.length();
	}
	return 0;
}

long long encrypt::FileInput::operator>>(std::string& data)
{
	if (this->fin.is_open()) {
		return this->fin >> data, data.length();
	}
	return 0;
}

encrypt::FileInput::~FileInput()
{
	if (this->fin.is_open()) {
		this->fin.close();
	}
	if (this->wfin.is_open()) {
		this->wfin.close();
	}
}
//==============================================================

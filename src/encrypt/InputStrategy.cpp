/*
   Copyright 2025 Htie digital

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

long long encrypt::StdoutInput::read(char* data,std::streamsize size)
{
	return std::cin.read(data,size), std::cin.gcount();
}
long long encrypt::StdoutInput::read(wchar_t* data, std::streamsize size)
{
	return std::wcin.read(data, size), std::wcin.gcount();
}
long long encrypt::StdoutInput::readln(std::string& data, char delim)
{
	return std::getline(std::cin, data, delim), std::cin.gcount();
}
long long encrypt::StdoutInput::readln(std::wstring& data, wchar_t delim)
{
	return std::getline(std::wcin, data, delim), std::wcin.gcount();
}
//==============================================================
encrypt::FileInput::FileInput(const char* path, int openType)
{
	this->fin.open(path, openType);
	if (!this->fin.is_open()) {
		throw T::i()->getMsg({L"iostrategy",1});
	}
}

encrypt::FileInput::FileInput(std::wstring path, int openType)
{
	this->wfin.open(path, openType);
	if (!this->wfin.is_open()) {
		throw T::i()->getMsg({L"iostrategy",1});
	}
	this->wfin.imbue(std::locale(".UTF-8"));//попытка исправить чтение с однобайтовго режима 
}

long long encrypt::FileInput::read(char* data, std::streamsize size)
{
	if (!this->fin.is_open()) {
		throw T::i()->getMsg({L"iostrategy",2});
	}
	return this->fin.read(data, size), this->fin.gcount();
}

long long encrypt::FileInput::read(wchar_t* data, std::streamsize size)
{
	if (!this->wfin.is_open()) {
		throw T::i()->getMsg({L"iostrategy",2});
	}
	return this->wfin.read(data, size), this->wfin.gcount();
}

long long encrypt::FileInput::readln(std::string& data, char delim)
{
	if (!this->fin.is_open()) {
		throw T::i()->getMsg({L"iostrategy",2});
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
		throw T::i()->getMsg({L"iostrategy",2});
	}
	if (std::getline(this->wfin, data, delim)) {
		return data.length();
	} else {
		return 0;
	}
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

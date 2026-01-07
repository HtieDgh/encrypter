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
#include "OutputStrategy.h"
#include "Translator.h"
using T = encrypt::Translator;

void encrypt::StdoutOutput::write(char* data, std::streamsize size)
{
	std::cout.write(data, size);
}

void encrypt::StdoutOutput::write(wchar_t* data, std::streamsize size)
{
	std::wcout.write(data, size);
}

void encrypt::StdoutOutput::write(std::wstring data)
{
	std::wcout << data;
}

encrypt::FileOutput::FileOutput(const char* path, int openType)
{
	this->fout.open(path, openType);
	if (!this->fout.is_open()) {
		throw T::i()->getMsg({L"iostrategy",1});
	}
}

encrypt::FileOutput::FileOutput(const wchar_t* path, int openType)
{
	this->wfout.open(path, openType);
	if (!this->wfout.is_open()) {
		throw T::i()->getMsg({L"iostrategy",1});
	}
}

void encrypt::FileOutput::write(char* data, std::streamsize size)
{
	if (this->fout.is_open()) {
		this->fout.write(data, size);
	}
	else {
		throw T::i()->getMsg({L"iostrategy",3});
	}
}

void encrypt::FileOutput::write(wchar_t* data, std::streamsize size)
{
	if (this->wfout.is_open()) {
		this->wfout.write(data, size);
	} else {
		throw T::i()->getMsg({L"iostrategy",3});
	}
}

void encrypt::FileOutput::write(std::wstring data)
{
	if (this->wfout.is_open()) {
		this->wfout << data;
	} else {
		throw T::i()->getMsg({L"iostrategy",3});
	}
}

encrypt::FileOutput::~FileOutput()
{
	if (this->fout.is_open()) {
		this->fout.close();
	}
}

void encrypt::StderrOutput::write(char* data, std::streamsize size)
{
	std::cerr.write(data, size);
}

void encrypt::StderrOutput::write(wchar_t* data, std::streamsize size)
{
	std::wcerr.write(data, size);
}

void encrypt::StderrOutput::write(std::wstring data)
{
	std::wcerr << data;
}

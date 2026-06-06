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
#include "Algorithm.h"
#include<codecvt>
#pragma warning(disable: 4996)   //отключения предупреждения о deprecated
void encrypt::Algorithm::setOutput(OutputStrategy* s)
{
	if (this->_of) {
		delete this->_of;
	}
	this->_of= s;
}

void encrypt::Algorithm::setInput(InputStrategy* s)
{
	if (this->_if) {
		delete this->_if;
	}
	this->_if= s;
}

void encrypt::Algorithm::setErrOutput(OutputStrategy* s)
{
	if (this->_ef) {
		delete this->_ef;
	}
	this->_ef = s;
}


std::wstring encrypt::Algorithm::s2ws(const std::string& str)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.from_bytes(str);
}

std::string encrypt::Algorithm::ws2s(const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}

encrypt::Algorithm::~Algorithm()
{
	if (this->_of) {
		delete this->_of;
	}
	if (this->_if) {
		delete this->_if;
	}
	if (this->_ef) {
		delete this->_ef;
	}
}

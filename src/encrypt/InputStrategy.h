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
#pragma once
#include<iostream>
#include<fstream>
#include<filesystem>
#include <string>
namespace fs = std::filesystem;
namespace encrypt {

	class InputStrategy
	{
	public:
		virtual ~InputStrategy() = default;
		// Возвращает количество прочитанных бит начиная с младшего индекса [data]
		virtual long long read(char* data, std::streamsize size = 1) = 0;
		virtual long long read(wchar_t* data, std::streamsize size = 1) = 0;
		virtual long long readln(std::string& data, char delim = '\n') = 0;
		virtual long long readln(std::wstring& data, wchar_t delim = '\n') = 0;
	};
	class StdoutInput : public InputStrategy {
	public:
		long long read(char* data, std::streamsize size = 1) override;
		long long read(wchar_t* data, std::streamsize size = 1) override;
		long long readln(std::string& data, char delim = '\n') override;
		long long readln(std::wstring& data, wchar_t delim = '\n') override;
	};

	class FileInput : public InputStrategy {
	private:
		std::ifstream fin;
		std::wifstream wfin;
	public:
		//path - путь до файла, openType - число, определенное в std::ios
		FileInput(const char* path, int openType = std::ios::binary);
		FileInput(std::wstring path, int openType = std::ios::binary);
		long long read(char* data, std::streamsize size = 1) override;
		long long read(wchar_t* data, std::streamsize size = 1) override;
		long long readln(std::string& data, char delim = '\n') override;
		long long readln(std::wstring& data, wchar_t delim = '\n') override;
		~FileInput();
	};
}

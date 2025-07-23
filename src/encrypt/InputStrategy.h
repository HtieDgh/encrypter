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
namespace fs = std::filesystem;
namespace encrypt {

	class InputStrategy
	{
	public:
		virtual ~InputStrategy() = default;
		// ���������� ���������� ���������� ��� ������� � �������� ������� [data]
		virtual long long read(char* data, std::streamsize size = 1) = 0;
	};
	class StdoutInput : public InputStrategy {
	public:
		long long read(char* data, std::streamsize size = 1) override;
	};

	class FileInput : public InputStrategy {
	private:
		std::ifstream fin;
	public:
		//path - ���� �� �����, openType - �����, ������������ � std::ios
		FileInput(const char* path, int openType = std::ios::binary);
		long long read(char* data, std::streamsize size = 1) override;
		~FileInput();
	};

	//
	class DirectoryInput : public InputStrategy {
	private:	
		FileInput* fin;
		
		void _getFilepaths(fs::path& fs_dirpath);
	public:
		DirectoryInput(const char* dirPath, int openType = std::ios::binary);
		long long read(char* data, std::streamsize size = 1) override;
		~DirectoryInput();
	};
}

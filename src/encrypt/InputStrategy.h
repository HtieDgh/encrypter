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
namespace encrypt {
	class InputStrategy
	{
	public:
		virtual ~InputStrategy() = default;
		virtual void read(char* data, std::streamsize size = 1) = 0;
	};
	class StdoutInput : public InputStrategy {
	public:
		void read(char* data, std::streamsize size = 1) override;
	};

	class FileInput : public InputStrategy {
	private:
		std::ifstream fin;
	public:
		//path - путь до файла, openType - число, определенное в std::ios
		FileInput(const char* path, std::ios_base::open_mode openType = std::ios::binary);
		void read(char* data, std::streamsize size = 1) override;
		~FileInput();
	};
}

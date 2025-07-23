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
	//Стратегия для разных выводов
	class OutputStrategy {
	public:
		virtual ~OutputStrategy() = default;
		virtual void write(char* data, std::streamsize size = 1) = 0;
	};

	class StdoutOutput : public OutputStrategy {
	public:
		void write(char* data, std::streamsize size = 1) override;
	};

	class StderrOutput : public OutputStrategy {
	public:
		void write(char* data, std::streamsize size = 1) override;
	};

	class FileOutput : public OutputStrategy {
	private:
		std::ofstream fout;
	public:
		
		//path - путь до файла, openType - число, определенное в std::ios
		FileOutput(const char* path, int openType = std::ios::binary);
		void write(char* data, std::streamsize size = 1) override;
		~FileOutput();
	};
};
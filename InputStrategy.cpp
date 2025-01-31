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

void encrypt::StdoutInput::read(char* data,std::streamsize size)
{
	std::cin.read(data,size);
}

encrypt::FileInput::FileInput(const char* path,std::ios_base::open_mode openType)
{
	this->fin.open(path, openType);
	if (!this->fin.is_open()) {
		throw std::runtime_error((const std::string)"Failed to open file: " + path+'\n');
	}
}

void encrypt::FileInput::read(char* data, std::streamsize size)
{
	if (this->fin.is_open()) {
		if (!this->fin.read(data, size)) {
			throw std::runtime_error("input file is ended\n");
		}
	} else {
		throw std::runtime_error("Failed to read file: file is not exist?\n");
	}
}

encrypt::FileInput::~FileInput()
{
	if (this->fin.is_open()) {
		this->fin.close();
	}
}

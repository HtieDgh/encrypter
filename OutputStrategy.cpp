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

void encrypt::StdoutOutput::write(char* data, std::streamsize size)
{
	std::cout.write(data, size);
}

encrypt::FileOutput::FileOutput(const char* path, std::ios_base::open_mode openType)
{
	this->fout.open(path, openType);
	if (!this->fout.is_open()) {
		throw std::runtime_error((const std::string)"Failed to open file: " + path+'\n');
	}
}

void encrypt::FileOutput::write(char* data, std::streamsize size)
{
	if (this->fout.is_open()) {
		this->fout.write(data, size);
	}
	else {
		throw std::runtime_error("Failed to write file: file is not exist?\n");
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
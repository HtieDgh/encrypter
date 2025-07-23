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

long long encrypt::StdoutInput::read(char* data,std::streamsize size)
{
	return std::cin.read(data,size), std::cin.gcount();
}
//==============================================================
encrypt::FileInput::FileInput(const char* path, int openType)
{
	this->fin.open(path, openType);
	if (!this->fin.is_open()) {
		throw std::runtime_error((const std::string)"Failed to open file: " + path + '\n');//TR6
	}
}

long long encrypt::FileInput::read(char* data, std::streamsize size)
{
	if (!this->fin.is_open()) {
		throw std::runtime_error("Failed to read file: is file existing?\n");//TR8
	}
	return this->fin.read(data, size), this->fin.gcount();
}

encrypt::FileInput::~FileInput()
{
	if (this->fin.is_open()) {
		this->fin.close();
	}
}

//==============================================================
encrypt::DirectoryInput::~DirectoryInput()
{
	delete this->fin;
}
void encrypt::DirectoryInput::_getFilepaths(fs::path& fs_dirpath)
{
	//TODO
}
encrypt::DirectoryInput::DirectoryInput(const char* dirPath, int openType)
{
	//this->fin = new FileInput[10]{FileInput()}
	//TODO
	this->fin=new FileInput(dirPath, openType);
}
long long encrypt::DirectoryInput::read(char* data, std::streamsize size)
{
	return this->fin[0].read(data);//TODO
}

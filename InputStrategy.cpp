#include "InputStrategy.h"

void encrypt::StdoutInput::read(char* data,std::streamsize size)
{
	std::cin.read(data,size);
}

encrypt::FileInput::FileInput(const char* path,std::ios_base::open_mode openType)
{
	this->fin.open(path, openType);
	if (!this->fin.is_open()) {
		throw std::runtime_error((const std::string)"Failed to open file: " + path);
	}
}

void encrypt::FileInput::read(char* data, std::streamsize size)
{
	if (this->fin.is_open()) {
		if (!this->fin.read(data, size)) {
			throw std::runtime_error("input file is ended");
		};
	} else {
		throw std::runtime_error("Failed to read file: file is not exist?");
	}
}

encrypt::FileInput::~FileInput()
{
	if (this->fin.is_open()) {
		this->fin.close();
	}
}

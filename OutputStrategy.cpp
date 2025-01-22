#include "OutputStrategy.h"

void encrypt::StdoutOutput::write(char* data)
{
	std::cout << *data;
}

void encrypt::StdoutOutput::write(char* data, std::streamsize size = 1)
{
	std::cout.write(data, size);
}

encrypt::FileOutput::FileOutput(const char* path, std::ios_base::open_mode openType)
{
	this->fout.open(path, openType);
	if (!this->fout.is_open()) {
		throw std::runtime_error((const std::string)"Failed to open file: " + path);
	}
}

void encrypt::FileOutput::write(char* data)
{
	if (this->fout.is_open()) {
		this->fout.write(data,1);
	}
	else {
		throw std::runtime_error("Failed to write file: file is not exist?");
	}
}

encrypt::FileOutput::~FileOutput()
{
	if (this->fout.is_open()) {
		this->fout.close();
	}
}

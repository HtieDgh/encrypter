#include "OutputStrategy.h"

void encrypt::StdoutOutput::write(char* data, std::streamsize size)
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

void encrypt::FileOutput::write(char* data, std::streamsize size)
{
	if (this->fout.is_open()) {
		this->fout.write(data, size);
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

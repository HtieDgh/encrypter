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

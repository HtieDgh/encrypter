#pragma once
#include<iostream>
#include<fstream>
namespace encrypt {
	//��������� ��� ������ �������
	class OutputStrategy {
	public:
		virtual ~OutputStrategy() = default;
		virtual void write(char* data) = 0;
	};
	class StdoutOutput : public OutputStrategy {
	public:
		void write(char* data) override;
		void write(char* data, std::streamsize size);
		

	};
	class FileOutput : public OutputStrategy {
	private:
		std::ofstream fout;
	public:
		//path - ���� �� �����, openType - �����, ������������ � std::ios
		FileOutput(const char* path, std::ios_base::open_mode openType = std::ios::binary);
		void write(char* data) override;
		~FileOutput();
	};
}
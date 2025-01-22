#pragma once
#include"Algoritm.h"
namespace encrypt {
	class Ceasar : public Algorithm
	{
	private:
		bool _done;		//������ ����������
		bool _mode;		//true ���� ��������� ������������
		int _d, _kd;	//�������� ������ � �����
		const char* _key;		//��������� �� ����
		char _dec;		//������������� ��������� ���������� 1 �����
		size_t _maxsize;//���� ������ � ������ ����� �������� ������� ���������� ����������
	public:
		Ceasar();
		Ceasar(char* _key, size_t maxsize);
		~Ceasar();

		const char* key() const;
		size_t maxsize() const;
		void key(const char* _key);
		void maxsize(int maxsize);

		void run() override;
		bool mode() const override;
		void mode(bool isEnc) override;
	};
}
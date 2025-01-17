#pragma once
#include"Algoritm.h"
namespace encrypt {
	class Ceaser : public Algoritm
	{
	private:
		bool _done;		//������ ����������
		bool _mode;		//true ���� ��������� ������������
		int _d, _kd;	//�������� ������ � �����
		char* _key;		//��������� �� ����
		char _dec;		//������������� ��������� ���������� 1 �����
		char _enc;		//���� ������� � ����������
		int _maxsize;//���� ������ � ������ ����� �������� ������� ���������� ����������
	public:
		Ceaser();
		Ceaser(char* _key, size_t maxsize);
		~Ceaser();

		char* key() const;
		size_t maxsize() const;
		void key(char* _key);
		void maxsize(int maxsize);

		void run() override;
		bool mode() const override;
		void mode(bool isEnc) override;
	};
}
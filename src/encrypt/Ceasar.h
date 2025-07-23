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
#pragma once
#include"Algorithm.h"
#include<map>
namespace encrypt {
	class Ceasar : public Algorithm
	{
	private:
		bool _mode;			//true ���� ��������� ������������
		int _kd;			//�������� �����, ������������ � run()
		const char* _key;	//��������� �� ����-������
		char _dec;			//������������� ��������� ���������� 1 �����
		size_t _d,			//������� �������� ������ � ������, ������������ � run()
			_maxsize;		//���� ������ � ������ ����� �������� ������� ���������� ����������
		bool _ready = false;//true ���� �������� ��������������� � ����� � ���������. ������������ � run() ����� �������� ���������� ���� ��� �� �������� �������� � ������ ������� ����������� modename
	public:
		Ceasar();
		Ceasar(char* modename, std::map<std::string, const char*>& params,OutputStrategy* const errout);
		~Ceasar();

		const char* key() const;
		size_t maxsize() const;
		void key(const char* _key);
		void maxsize(size_t maxsize);

		void run() override;
		bool mode() const;
		void mode(bool isEnc);
		void readme() override;
	};
}
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
#include<string>
#include<iostream>
#include<iomanip>
#include<locale.h>
#include<Windows.h>
#include<map>
#include "Ceasar.h"
#include "AlgorithmStrategy.h"

using namespace std;
using namespace encrypt;
#define README_PATH ""

// ��������� ���������� ������

// ����� �������
void readme(std::ostream& out) {
	out << "�������������: encrypter {��� ���������} {dec|enc} [�����] < \\path\\to\\file.txt > \\path\\to\\encrypted.txt\n";
}
void Creadme(std::ostream& out) {
	out << "������������� ceasar:\n"
		<< "\tencrypter ceasar enc -k \"key to encrypt\" -size 10 -of \\path\\to\\encrypted.txt -if \\path\\to\\file.txt\n"
		<< "\n"
		<< "���\n"
		<< "\n"
		<< "\tencrypter ceasar dec -k \"key to decrypt\" -size 10 -of \\path\\to\\decrypted.txt -if path\\to\\encrypted.txt\n"
		<< "\n"
		<< "��� ����������� ����� ������ ����� `-if` � `-of` ����� �� ���������:\n"
		<< "\n"
		<< "\tencrypter ceasar {dec|enc} -k \"key to decrypt\"";
}


int main(int argc, char* argv[], char* envp[]){
	setlocale(LC_ALL, ".1251");
	SetConsoleCP(1251);
	size_t inSize = 100;
	AlgorithmStrategy* controller=new AlgorithmStrategy();
	map<string,const char*> params;
	//����������� ���������� � �� ��������
	for (int i = 3; i < argc; i++) {
		if (argv[i][0] == '-' && i + 1 < argc && argv[i + 1][0] != '-')
		{
		// �������� �� ���������
			params[argv[i]] = argv[i + 1];
		}
		else if (argv[i][0] == '-')
		{
		// �������� ��� ��������, �������� ����
			params[argv[i]] = "";
		}
	}
	//����� �������
	if (argc <= 2)
	{
		readme(cout);
		return 0;
	} 

	// �������� ������
	if (!strcmp(argv[1], "ceasar")) {
		Ceasar* alg = new Ceasar();
		controller->setAlgorithm(alg);

		if (!strcmp(argv[2], "enc")) 
		{
			alg->mode(false);
		} 
		else if (!strcmp(argv[2], "dec")) 
		{
			alg->mode(true);
		} else {
			Creadme(cout);
			return 0;
		}

	// ��������� ���������� ��������� ������ � ���������� � ����������
		try
		{
			if (params.count("-k") == 0)
			{
				throw std::logic_error("�������� -k �� �����");
			}

			alg->key(params["-k"]);

			if (params.count("-size") == 0)
			{
				throw std::logic_error("�������� -size �� �����");
			}

			alg->maxsize(atoi(params["-size"])); //atoi() ��������� char* � int
			
			// ���������� ������
			if (params.count("-of") == 0)
			{
				// �� ��������� ����� � stdout
				alg->setOutput(new StdoutOutput());
			} else {
				// ���� �� ����� ��������, ����� � ���� ����
				alg->setOutput(new FileOutput(params["-of"]));
			}

			// ���������� �����
			if (params.count("-if") == 0) 
			{
				// �� ��������� ����� � stdout
				alg->setInput(new StdoutInput());
			} else {
				// ���� �� ����� ��������, ���� �� �����
				alg->setInput(new FileInput(params["-if"]));
			}
		
			// ���������� �������
			alg->run();
			return 0;
		}
		catch (const std::exception& e)
		{
			cerr << e.what() << endl;
			Creadme(cout);
		}
	} else {
		readme(cout);
		return 0;
	}
}
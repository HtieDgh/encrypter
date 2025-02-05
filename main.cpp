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
#include "src\encrypt\Ceasar.h"
#include "src\encrypt\XTEA.h"
#include "src\encrypt\AlgorithmStrategy.h"

using namespace std;
using namespace encrypt;

int main(int argc, char* argv[], char* envp[]){
	setlocale(LC_ALL, ".1251");
	SetConsoleCP(1251);
	AlgorithmStrategy* controller=new AlgorithmStrategy();
	StderrOutput* errout = new StderrOutput();
	map<string,const char*> params;
	// ����������� ���������� � �� ��������
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
	// ����� ������� ��� ���������� ����������
	if (argc <= 2)
	{
		controller->readme(errout);
		delete controller;
		return 0;
	} 

	// ���������� � ����������: ��������� ������ � ������������� �������-���������� �����������
	if (!strcmp(argv[1], "ceasar"))			// �������� ������
	{
		
		// ������������� ���������
		controller->setAlgorithm(new Ceasar(argv[2], params, errout));

	} else if (!strcmp(argv[1], "xtea"))	// XTEA
	{

		// ������������� ���������
		controller->setAlgorithm(new XTEA(argv[2], params, errout));

	}else{
		controller->readme(errout);
	}
	// ���������� ���������
	controller->doAlgorithm();
	delete controller;
	return 0;
}
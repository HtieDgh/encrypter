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
	StderrOutput* errout = new StderrOutput();//TODO
	map<string,const char*> params;
	// ОПРЕДЕЛЕНИЕ ПАРАМЕТРОВ И ИХ ЗНАЧЕНИЙ
	for (int i = 3; i < argc; i++) {
		if (argv[i][0] == '-' && i + 1 < argc && argv[i + 1][0] != '-')
		{
		// параметр со значением
			params[argv[i]] = argv[i + 1];
		}
		else if (argv[i][0] == '-')
		{
		// параметр без значения, например флаг
			params[argv[i]] = "";
		}
	}
	// ВЫВОД СПРАВКИ ПРИ НЕДОСТАТКЕ АРГУМЕНТОВ
	if (argc <= 2)
	{
		controller->readme(errout);
		delete controller;
		return 0;
	} 

	// ПОДГОТОВКА К ВЫПОЛНЕНИЮ: НАСТРОЙКА РЕЖИМА В КОНСТРУКТОРАХ КЛАССОВ-РЕАЛИЗАЦИЙ АЛГОРИТМАОВ
	if (!strcmp(argv[1], "ceasar"))			// АГЛОРИТМ ЦЕЗАРЯ
	{
		
		// Подтверждение алгоритма
		controller->setAlgorithm(new Ceasar(argv[2], params, errout));

	} else if (!strcmp(argv[1], "xtea"))	// XTEA
	{

		// Подтверждение алгоритма
		controller->setAlgorithm(new XTEA(argv[2], params, errout));

	}else{
		controller->readme(errout);
	}
	// ВЫПОЛНЕНИЕ АЛГОРИТМА
	controller->doAlgorithm();
	delete controller;
	return 0;
}
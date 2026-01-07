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
#include "src\encrypt\EncrypterConfig.h"
#include "src\encrypt\ConfigParser.h"
#include "src\encrypt\Translator.h"

#define DEFAULT_LC_PATH  L"./lc"
#define DEFAULT_LC_EXT  L".lc"
#define DEFAULT_CFG_PATH L"./config.cfg"

using namespace std;
using namespace encrypt;
using T = encrypt::Translator;

int main(int argc, char* argv[], char* envp[]){
	setlocale(LC_ALL, ".utf8");
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);

	AlgorithmStrategy* controller=new AlgorithmStrategy();
	StderrOutput* errout = new StderrOutput();
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

	//ЧТЕНИЕ CONFIG ФАЙЛА
		FileInput* FI = nullptr;
		try
		{
			FI = new FileInput(DEFAULT_CFG_PATH);
		
		} catch (const std::wstring&e)
		{
			errout->write(e);
			controller->readme(errout);
			return -1;
		}
		EncrypterConfigParser* ECP = new EncrypterConfigParser(FI);
		delete FI;

	// ОПРЕДЕЛЕНИЕ ЯЗЫКА ИНФОРМАЦИОННЫХ СООБЩЕНИЙ
		wstring locale = DEFAULT_LC_PATH; locale += L'/' + ECP->getWstring(L"locale", L"ru-RU") + DEFAULT_LC_EXT;
		T::i(new FileInput(locale), new TranslatorParser());

	// ВЫВОД СПРАВКИ ПРИ НЕДОСТАТКЕ АРГУМЕНТОВ
		if (argc <= 2)
		{
			controller->readme(errout);
			delete controller;
			return 0;
		}
	// ПОДГОТОВКА К ВЫПОЛНЕНИЮ: НАСТРОЙКА РЕЖИМА В КОНСТРУКТОРАХ КЛАССОВ-РЕАЛИЗАЦИЙ
		if (!strcmp(argv[1], "ceasar"))			// АГЛОРИТМ ЦЕЗАРЯ
		{
			// Подтверждение алгоритма
			controller->setAlgorithm(new Ceasar(argv[2], params, errout));

		} else if (!strcmp(argv[1], "xtea"))	// XTEA
		{
			// Подтверждение алгоритма
			controller->setAlgorithm(new XTEA(argv[2], params, errout));

		} else if (!strcmp(argv[1], "config"))	// CONFIG
		{
			controller->setAlgorithm(new EncrypterConfig(
				argv[2],
				params,
				errout,
				L"config.cfg",
				ECP
			));

		}else
		{
			controller->readme(errout);
		}

	// ВЫПОЛНЕНИЕ АЛГОРИТМА
		controller->doAlgorithm();
		delete controller;
	return 0;
}
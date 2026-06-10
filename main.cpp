/*
   Copyright 2026 Htie digital

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
#include "src\encrypt\Controller.h"
#include "src\encrypt\EncrypterConfig.h"
#include "src\encrypt\ConfigParser.h"
#include "src\encrypt\Translator.h"
#include "src\encrypt\InterpretationMode.h"
#include "src\encrypt\EncryptReadme.h"
#include "src\encrypt\General.h"
#include "src\encrypt\KeystreamGenerator.h"

#define DEFAULT_LC_PATH  L"./lc"
#define DEFAULT_LC_EXT  L".lc"
#define DEFAULT_CFG_PATH L"./config.cfg"
#define DEFAULT_CFG_CONTENT LR"end(locale=en-US
)end"

using namespace std;
using namespace encrypt;
using T = encrypt::Translator;
using G = encrypt::General;


int wmain(int argc, wchar_t* argv[], wchar_t* envp[]){
	setlocale(LC_ALL, ".utf8");
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);

	Controller* controller=new Controller();
	StderrOutput* errout = new StderrOutput();

	// ОПРЕДЕЛЕНИЕ ПАРАМЕТРОВ И ИХ ЗНАЧЕНИЙ
		auto params = G::buildParams(argv, argc);

	//ЧТЕНИЕ CONFIG ФАЙЛА
		FileInput* FI = nullptr;
		try
		{
			FI = new FileInput(DEFAULT_CFG_PATH);
		
		} catch (...)
		{
			errout->write(L"config file can't be opened, creating default");//TODO default strings should be hardcoded?
			auto cfgOut=new FileOutput(DEFAULT_CFG_PATH);
			cfgOut->write(DEFAULT_CFG_CONTENT);
			delete cfgOut;
		}
		EncrypterConfigParser* ECP = new EncrypterConfigParser(FI);
		delete FI;

	// ОПРЕДЕЛЕНИЕ ЯЗЫКА ИНФОРМАЦИОННЫХ СООБЩЕНИЙ
		wstring locale = DEFAULT_LC_PATH; locale += L'/' + ECP->getWstring(L"locale", L"en-US") + DEFAULT_LC_EXT;
		T::i(new FileInput(locale), new TranslatorParser());



	// ПОДГОТОВКА К ВЫПОЛНЕНИЮ: ВЫБОР КЛАССА РЕАЛИЗАЦИИ

		// ВЫВОД СПРАВКИ ПРИ НЕДОСТАТКЕ АРГУМЕНТОВ
		if (argc <= 2)
		{
			controller->setAlgorithm(new EncryptReadme(errout));

		} else if ( G::areEqual(argv[1], L"ceasar") )			// АГЛОРИТМ ЦЕЗАРЯ
		{
			controller->setAlgorithm(new Ceasar(argv[2], params, errout));

		} else if ( G::areEqual(argv[1], L"xtea") )	// XTEA
		{
			controller->setAlgorithm(new XTEA(argv[2], params, errout));

		} else if ( G::areEqual(argv[1], L"config") )	// CONFIG
		{
			controller->setAlgorithm(new EncrypterConfig(
				argv[2],
				params,
				errout,
				L"config.cfg",
				ECP
			));

		} else if ( G::areEqual(argv[1], L"i") ) 
		{
			controller->setAlgorithm(new InterpretationMode(
				argv[2],
				new StdInput(),
				new StderrOutput(),
				errout
			));
		} else if (G::areEqual(argv[1], L"chacha"))
		{
			controller->setAlgorithm(new KeystreamGenerator(argv[2], params, errout));
		}
		else
		{
			controller->setAlgorithm(new EncryptReadme(errout, EncryptReadme::PRESTATE::noalg));
		}

	// ВЫПОЛНЕНИЕ АЛГОРИТМА
		controller->doAlgorithm();

		delete controller;
	return 0;
}


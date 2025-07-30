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
#include "Ceasar.h"
#include<iostream>
#include<iomanip>
#include<sstream>
using namespace std;
//TR<номер> - идентификатор для Модуля локализации. //TODO

encrypt::Ceasar::Ceasar()
{
	_mode = encrypt::Ceasar::CeasarMode::README;
	_d = 0, _kd = 0;
	this->key(new char[8] {"default"});
	_dec = 0;
	this->maxsize(0);
	this->setErrOutput(new StderrOutput());
}

encrypt::Ceasar::Ceasar(char* modename, map<string, const char*>& params, OutputStrategy* const errout)
{
	this->setErrOutput(errout);
	try {
		if (!strcmp(modename, "enc"))
		{
			this->mode(encrypt::Ceasar::CeasarMode::ENC);
		}
		else if (!strcmp(modename, "dec"))
		{
			this->mode(encrypt::Ceasar::CeasarMode::DEC);
		} 
		else if (!strcmp(modename, "-?") || !strcmp(modename, "-help")) 
		{
			throw encrypt::Ceasar::CeasarMode::README;
		} 
		else 
		{
			throw encrypt::Ceasar::CeasarMode::NOMODE;
		}

		if (params.count("-k") == 0)
		{
			throw encrypt::Ceasar::CeasarMode::NOKEY;
		}

		this->key(params["-k"]);

		if (params.count("-size") == 0)
		{
			this->maxsize(0);
		} else {
			this->maxsize(atoi(params["-size"]));//atoi() переводит char* в int
		}

		// НАЗНАЧЕНИЕ ВЫВОДА
		if (params.count("-of") == 0)
		{
			// По умолчанию вывод в stdout
			this->setOutput(new StdoutOutput());
		} else {
			// Путь до файла назначен, вывод в этот файл
			this->setOutput(new FileOutput(params["-of"]));
		}

		// НАЗНАЧЕНИЕ ВВОДА
		if (params.count("-if") == 0)
		{
			// По умолчанию вывод в stdout
			this->setInput(new StdoutInput());
		} else {
			// Путь до файла назначен, ввод из файла
			this->setInput(new FileInput(params["-if"]));
		}
	} catch (const encrypt::Ceasar::CeasarMode& e) {
		this->mode(e);
	}
}

const char* encrypt::Ceasar::key() const
{
	return this->_key;
}

size_t encrypt::Ceasar::maxsize() const
{
	return this->_maxsize;
}

void encrypt::Ceasar::key(const char* _key)
{
	this->_key = _key;
}

void encrypt::Ceasar::maxsize(size_t maxsize)
{
	this->_maxsize = maxsize;
}

void encrypt::Ceasar::run()
{
	switch (_mode)
	{
	case encrypt::Ceasar::CeasarMode::ENC:
	case encrypt::Ceasar::CeasarMode::DEC:
		try {
			while (true)
			{
				// ВВОД
				if (this->_if->read(&_dec) != 1) {
					break;
				}

				// ОБРАБОТКА
				_dec += ( _mode == encrypt::Ceasar::CeasarMode::DEC ? -1 : 1) * _key[_kd++];

				if (_kd == strlen(_key)) {
					_kd = 0;
				}

				// ВЫВОД
				this->_of->write(&_dec);

				// СМЕНА ИТЕРАЦИИ _maxsize - количество байт которые надо считать с потока ввода
				// В случае если -size опция не была задана _maxsize=0 и это условие всегда ложно,
				// выход из цикла по окончанию входного потока
				if (++_d == _maxsize) {
					break;
				}
			}
		} catch (const std::exception& e)
		{
			cerr << e.what() << endl;
			this->readme();
		}
		break;
	case encrypt::Ceasar::CeasarMode::NOMODE:
		this->_ef->write((char*)("Такого режима не существует\n"), 28);//TR1
		this->readme();
		break;
	case encrypt::Ceasar::CeasarMode::README:
		this->_ef->write((char*)("Вывод справки по использованию:\n"), 32);//TR2
		this->readme();
		break;
	case encrypt::Ceasar::CeasarMode::NOKEY:
		this->_ef->write((char*)("Параметр -k не задан\n"), 21);//TR3
		this->readme();
		break;
	}
}

encrypt::Ceasar::CeasarMode encrypt::Ceasar::mode() const
{
	return this->_mode;
}

void encrypt::Ceasar::mode(encrypt::Ceasar::CeasarMode mode)
{
	this->_mode = mode;
}

void encrypt::Ceasar::readme()
{
	stringstream s;//TR5
	s << "\nДля того, чтобы зашифровать файл выполните:\n"
		<< "\n"
		<< "\tencrypter ceasar enc -k \"key to encrypt\" -size 10 -of \\path\\to\\encrypted.txt -if \\path\\to\\file.txt\n"
		<< "\n"
		<< "Для расшифровки:\n"
		<< "\n"
		<< "\tencrypter ceasar dec -k \"key to decrypt\" -size 10 -of \\path\\to\\decrypted.txt -if path\\to\\encrypted.txt\n"
		<< "\n"
		<< "Для консольного ввода вывода опции `-if` и `-of` можно не указывать:\n"
		<< "\n"
		<< "\tencrypter ceasar {dec|enc} -k \"key to decrypt\"\n"
		<< "\n"
		<< "Для корректной расшифровки опции -size (если использовалась) и -k должны повторять значения при шифровании\n";
	this->_ef->write((char*)s.str().c_str(), s.str().size());
}

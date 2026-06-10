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
#include "Ceasar.h"
#include<iostream>
#include<iomanip>
#include"Translator.h"
#include"General.h"

using G = encrypt::General;
using T = encrypt::Translator;

using namespace std;

encrypt::Ceasar::Ceasar(std::wstring modename, std::map<std::wstring, std::wstring>& params, OutputStrategy* const errout)
{
	this->setErrOutput(errout);
	try {
		if (G::areEqual(modename, L"enc"))
		{
			this->mode(encrypt::Ceasar::CeasarMode::ENC);
		} else if (G::areEqual(modename, L"dec"))
		{
			this->mode(encrypt::Ceasar::CeasarMode::DEC);
		} else if (G::areEqual(modename, L"-?") || G::areEqual(modename, L"-help"))
		{
			throw encrypt::Ceasar::CeasarMode::README;
		} else
		{
			throw encrypt::Ceasar::CeasarMode::NOMODE;
		}
		//ЧТЕНИЕ КЛЮЧА
		if (params.count(L"-k") == 0 && params.count(L"-kf")==0)
		{
			throw encrypt::Ceasar::CeasarMode::NOKEY;
		}
		if (params.count(L"-kf") != 0) {
			this->setKeyFile(new FileInput(ws2s(params[L"-kf"]).c_str()));
			this->_key = new char[1];
		} else {
			//перевод wchar
			this->_keysize = params[L"-k"].size() * sizeof(wchar_t);
			this->_key = new char[this->_keysize + 1] {};
			memcpy(this->_key, params[L"-k"].c_str(), this->_keysize);
			//this->key( params[L"-k"] );
		}
		

		if (params.count(L"-size") != 0)
		{
			this->maxsize(_wtoll(params[L"-size"].c_str()));//_wtoll() переводит wchar_t* в long long
		}
		else if(params.count(L"-s") != 0)
		{
			this->maxsize(_wtoll(params[L"-s"].c_str()));
		}
		else {
			this->maxsize(0);
		}

		// НАЗНАЧЕНИЕ ВЫВОДА
		if (params.count(L"-of") == 0)
		{
			// По умолчанию вывод в stdout
			this->setOutput(new StdOutput());
		} else {
			// Путь до файла назначен, вывод в этот файл
			this->setOutput(new FileOutput(ws2s(params[L"-of"]).c_str()));
		}

		// НАЗНАЧЕНИЕ ВВОДА
		if (params.count(L"-if") == 0)
		{
			// По умолчанию вывод в stdout
			this->setInput(new StdInput());
		} else {
			// Путь до файла назначен, ввод из файла
			this->setInput(new FileInput(ws2s(params[L"-if"]).c_str()));
		}
	} catch (const encrypt::Ceasar::CeasarMode& e)
	{
		this->mode(e);
	} catch (const std::wstring& e)
	{ 
		// В случае отсутвия доступа к файлу
		this->_ef->write(e);
		this->mode(encrypt::Ceasar::CeasarMode::README);
	}
}

char* encrypt::Ceasar::key() const
{
	return this->_key;
}

size_t encrypt::Ceasar::maxsize() const
{
	return this->_maxsize;
}

void encrypt::Ceasar::key(char* _key)
{
	this->_key = _key;
}

void encrypt::Ceasar::maxsize(size_t maxsize)
{
	this->_maxsize = maxsize;
}

void encrypt::Ceasar::setKeyFile(InputStrategy* kf)
{
	if (this->_keyFile) {
		delete this->_keyFile;
	}
	this->_keyFile = kf;
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
				_dec += (_mode == encrypt::Ceasar::CeasarMode::DEC ? -1 : 1) * _nextKey();

				// ВЫВОД
				this->_of->write(&_dec);

				// СМЕНА ИТЕРАЦИИ _maxsize - количество байт которые надо считать с потока ввода
				// В случае если -size опция не была задана _maxsize=0 и это условие всегда ложно,
				// выход из цикла по окончанию входного потока
				if (++_d == _maxsize) {
					break;
				}
			}
		} catch (const std::wstring& e)
		{
			// В случае отсутвия доступа к файлу
			this->_ef->write(e);
			this->readme();
		}
		break;
	case encrypt::Ceasar::CeasarMode::NOMODE:
	case encrypt::Ceasar::CeasarMode::README:
	case encrypt::Ceasar::CeasarMode::NOKEY:
		this->_ef->write(T::msg({L"ceasar",(size_t)_mode}));//свести enum к числу-идентификатору строки
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
	this->_ef->write(T::msg({L"ceasar",4}));
}

encrypt::Ceasar::~Ceasar()
{
	delete[] _key;
	if (_keyFile) delete _keyFile;
}

char encrypt::Ceasar::_nextKey()
{
	//если _keyFile назначен то выодить следующий ключ из него, иначе из _key
	if (this->_keyFile == nullptr) {
		if (_kd == _keysize) {
			_kd = 0;
		}
		return _key[_kd++];
	} else {
		if (this->_keyFile->read(_key) != 1) {
			this->_keyFile->reset();
			this->_keyFile->read(_key);
		};
		return _key[0];
	}

}

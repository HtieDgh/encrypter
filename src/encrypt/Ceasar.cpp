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
#include<bitset> // ��� ��������� ������������� �����
using namespace std;

encrypt::Ceasar::Ceasar()
{
	_done = false;
	_mode = false;
	_d = 0, _kd = 0;
	this->key(new char[8] {"default"});
	_dec = 0;
	this->maxsize(-1);
	this->setErrOutput(new StderrOutput());
}

encrypt::Ceasar::Ceasar(char* modename, std::map<string, const char*>& const params, OutputStrategy* const errout)
{
	this->setErrOutput(errout);
	try
	{
		if (!strcmp(modename, "enc"))
		{
			this->mode(false);
		} else if (!strcmp(modename, "dec"))
		{
			this->mode(true);
		} else if (!strcmp(modename, "-?") || !strcmp(modename, "-help")) {
			throw std::logic_error("����� ������� �� �������������:\n");
		} else {
			throw std::logic_error("������ ������ �� ����������");
		}

		if (params.count("-k") == 0)
		{
			throw std::logic_error("�������� -k �� �����");
		}

		this->key(params["-k"]);

		if (params.count("-size") == 0)
		{
			throw std::logic_error("�������� -size �� �����");
		}

		this->maxsize(atoi(params["-size"])); //atoi() ��������� char* � int

		// ���������� ������
		if (params.count("-of") == 0)
		{
			// �� ��������� ����� � stdout
			this->setOutput(new StdoutOutput());
		} else {
			// ���� �� ����� ��������, ����� � ���� ����
			this->setOutput(new FileOutput(params["-of"]));
		}

		// ���������� �����
		if (params.count("-if") == 0)
		{
			// �� ��������� ����� � stdout
			this->setInput(new StdoutInput());
		} else {
			// ���� �� ����� ��������, ���� �� �����
			this->setInput(new FileInput(params["-if"]));
		}
	} catch (const std::exception& e)
	{
		this->_ef->write((char*)e.what(),strlen(e.what()));
		this->readme();
	}
}

encrypt::Ceasar::~Ceasar()
{
	delete[] _key;
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
	try {
		while (!_done)
		{
			// ����
			this->_if->read(&_dec);

			// ���������
			_dec += ( _mode ? -1 : 1)*_key[_kd++];
		
			if (_kd == strlen(_key)) {
				_kd = 0;
			}

			// �����
			this->_of->write(&_dec);

			// ����� �������� _maxsize - ���������� ���� ������� ���� ������� � ������ �����
			if (++_d == _maxsize) {
				_done = true;
			}
		}
	} catch (const std::exception& e)
	{
		cerr << e.what() << endl;
		this->readme();
	}
}

bool encrypt::Ceasar::mode() const
{
	return this->_mode;
}

void encrypt::Ceasar::mode(bool isEnc)
{
	this->_mode = isEnc;
}

void encrypt::Ceasar::readme()
{
	stringstream s;
	s << "��� ����, ����� ����������� ���� ���������:\n"
		<< "\n"
		<< "\tencrypter ceasar enc -k \"key to encrypt\" -size 10 -of \\path\\to\\encrypted.txt -if \\path\\to\\file.txt\n"
		<< "\n"
		<< "��� �����������:\n"
		<< "\n"
		<< "\tencrypter ceasar dec -k \"key to decrypt\" -size 10 -of \\path\\to\\decrypted.txt -if path\\to\\encrypted.txt\n"
		<< "\n"
		<< "��� ����������� ����� ������ ����� `-if` � `-of` ����� �� ���������:\n"
		<< "\n"
		<< "\tencrypter ceasar {dec|enc} -k \"key to decrypt\"\n"
		<< "\n"
		<< "��� ���������� ����������� ����� -size � -k ������ ��������� �������� ��� ����������\n";
	this->_ef->write((char*)s.str().c_str(), s.str().size());
}

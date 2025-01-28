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
}

encrypt::Ceasar::Ceasar(char* _key, size_t maxsize)
{
	_done = false;
	_d = 0, _kd = 0;
	this->key(_key);
	_dec = 0;
	this->maxsize(maxsize);
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
	size_t count = 0;
	while (!_done)
	{
		count++;
		// ����
		this->_if->read(&_dec);
		//debug cout << setw(3) << _d << "= " << std::bitset<8>(_dec) << "|" << _dec;

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
}

bool encrypt::Ceasar::mode() const
{
	return this->_mode;
}

void encrypt::Ceasar::mode(bool isEnc)
{
	this->_mode = isEnc;
}

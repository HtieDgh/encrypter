#include "Ceasar.h"
#include<iostream>
#include<iomanip>
#include<bitset> // для двоичного представления числа
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
		// ВВОД
		this->_if->read(&_dec);
		//debug cout << setw(3) << _d << "= " << std::bitset<8>(_dec) << "|" << _dec;

		// ОБРАБОТКА
		_dec += ( _mode ? -1 : 1)*_key[_kd++];
		
		if (_kd == strlen(_key)) {
			_kd = 0;
		}

		// ВЫВОД
		this->_of->write(&_dec);

		// СМЕНА ИТЕРАЦИИ _maxsize - количество байт которые надо считать с потока ввода
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

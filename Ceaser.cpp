#include "Ceaser.h"
#include<iostream>
#include<iomanip>
#include<bitset> // для двоичного представления числа
using namespace std;

encrypt::Ceaser::Ceaser()
{
	_done = false;
	_mode = false;
	_d = 0, _kd = 0;
	this->key(new char[8] {"default"});
	_dec = 0;
	_enc = 0;
	this->maxsize(-1);
}

encrypt::Ceaser::Ceaser(char* _key, size_t maxsize)
{
	_done = false;
	_d = 0, _kd = 0;
	this->key(_key);
	_dec = 0;
	_enc = 0;
	this->maxsize(maxsize);
}

encrypt::Ceaser::~Ceaser()
{
	delete[] _key;
}

char* encrypt::Ceaser::key() const
{
	return this->_key;
}

size_t encrypt::Ceaser::maxsize() const
{
	return this->_maxsize;
}

void encrypt::Ceaser::key(char* _key)
{
	this->_key = _key;
}

void encrypt::Ceaser::maxsize(int maxsize)
{
	this->_maxsize = maxsize;
}

void encrypt::Ceaser::run()
{
	while (!_done)
	{
		// ВВОД
		cin.get(_dec);
		//debug cout << setw(3) << _d << "= " << std::bitset<8>(_dec) << "|" << _dec;

		// ОБРАБОТКА
		if (_mode) {//РАСШИФРОВКА
			_dec -= _key[_kd++];
		}else {//ШИФРОВКА
			_dec += _key[_kd++];
		}
		
		if (_kd == strlen(_key)) {
			_kd = 0;
		}

		// ВЫВОД
		cout << _dec;

		// СМЕНА ИТЕРАЦИИ _maxsize - количество байт которые надо считать с потока ввода
		if (++_d == _maxsize) {
			_done = true;
		}
	}
}

bool encrypt::Ceaser::mode() const
{
	return this->_mode;
}

void encrypt::Ceaser::mode(bool isEnc)
{
	this->_mode = isEnc;
}

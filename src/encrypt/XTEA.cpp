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
#include "XTEA.h"
#include<random>
void encrypt::XTEA::_encode(uint32_t* v)
{
	unsigned int i;
	uint32_t v0 = v[0], v1 = v[1], sum = 0, delta = 0x9E3779B9;
	for (i = 0; i < _nr; i++) {

		v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + _key[sum & 3]);
		sum += delta;
		v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + _key[(sum >> 11) & 3]);
	}
	v[0] = v0;
	v[1] = v1;
}

void encrypt::XTEA::_decode(uint32_t* v)
{
	unsigned int i;
	uint32_t v0 = v[0], v1 = v[1], delta = 0x9E3779B9, sum = delta * _nr;
	for (i = 0; i < _nr; i++) {
		v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + _key[(sum >> 11) & 3]);
		sum -= delta;
		v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + _key[sum & 3]);
	}
	v[0] = v0;
	v[1] = v1;
}

uint32_t* encrypt::XTEA::_gen()
{
	if (_key) {
		delete[] this->_key;
	}
	_key = new uint32_t[4];

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<uint32_t> dist(0, UINT32_MAX);

	for (uint8_t i = 0; i < 4; i++) {
		_key[i]=dist(gen);
	}
	
	return _key;
}
encrypt::XTEA::XTEA()
{
	this->_mode = XTEAmode::ENC;
}

encrypt::XTEA::XTEA(char* modename, std::map<std::string, const char*>& const params, OutputStrategy* const errout)
{
	this->setErrOutput(errout);
	try
	{
		// ����� ������
		if (!strcmp(modename, "enc"))
		{
			this->_mode = XTEAmode::ENC;
		} else if (!strcmp(modename, "dec"))
		{
			this->_mode = XTEAmode::DEC;
		} else if (!strcmp(modename, "gen"))
		{
			this->_mode = XTEAmode::GEN;
			if (params.count("-of") == 0) {
				throw std::logic_error("������� gen ������� ������������� ������������� ����� -of\n");
			}
		} else if (
			!strcmp(modename, "-?") ||
			!strcmp(modename, "-help") ||
			(
				params.count("-kf") == 0 &&
				params.count("-of") == 0 &&
				params.count("-if") == 0
				)
			)
		{
			throw std::logic_error("����� ������� �� �������������:\n");
		} else {
			throw std::logic_error("����� ������� �� ����������\n");
		}

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
			// �� ��������� ���� �� stdin ����� ���� �� �����
			this->setInput(new StdoutInput());
		} else {
			this->setInput(new FileInput(params["-if"]));
		}

		if (_mode != XTEAmode::GEN) {
			if (params.count("-size") == 0)
			{
				throw std::logic_error("-size is not assigned\n");
			}
			this->_maxsize = atoi(params["-size"]); //atoi() ��������� char* � int
		}
		
		if (params.count("-nr") != 0)
		{
			this->_nr = atoi(params["-nr"]);
		}
		// ���������� ��� ��������� ������� � �����
		if (_mode == XTEAmode::ENC && params.count("-kf") == 0 && params.count("-of") == 0)
		{
			throw std::logic_error("can't use enc without -kf and -of\n");
		}
		if (_mode == XTEAmode::DEC && params.count("-kf") == 0 && params.count("-if") == 0)
		{
			throw std::logic_error("can't use dec without -kf and -if\n");
		}

		// ��� ������� ��� dec enc gen - _key ������ ���� �������� ����� �����������!
		if (params.count("-kf") != 0 && _mode != XTEAmode::GEN)
		{
			_key=reedKeyFile(params["-kf"]);
		}
		else if(
			params.count("-kf") == 0 && 
			_mode == XTEAmode::ENC
		){
			// ������������� ����-���� � ��� �� ���������� ��� � ��������� ���������� (-of)
			std::string s = params["-of"]; s += _KEYFILEEXT;
			FileOutput* okf = new FileOutput(s.c_str());
			this->gen(okf);
			delete okf;
		}
		else if (
			params.count("-kf") == 0 &&
			_mode == XTEAmode::DEC
		){
			// ������� ��������� ���� � ��� �� ���������� ��� � -if
			std::string s = params["-if"]; s += _KEYFILEEXT;
			_key = reedKeyFile(s.c_str());
		}
	} catch (const std::exception& e)
	{
		this->_ef->write((char*)e.what(), strlen(e.what()));
		_mode = XTEAmode::README;
		this->readme();
	}
}

encrypt::XTEA::~XTEA()
{
	delete[] _key;
}

void encrypt::XTEA::gen(OutputStrategy* okf)
{
	_gen();
	char out=0;
	for (int i = 0; i < 4; i++) {
		for (int j = 3; j >=0; j--) {
			out = (_key[i] >> j * 8) & 0xFF;
			okf->write(&out);
		}
	}
}

uint32_t* encrypt::XTEA::reedKeyFile(const char* path)
{
	uint32_t* out = new uint32_t[4];
	std::ifstream fin(path, std::ios::binary);

	if (!fin.is_open()) {
		throw std::logic_error("���� �� ������ ��� ��� ������� � ����-�����\n");
	}

	fin.read(reinterpret_cast<char*>(out), sizeof(out));

	if (fin.gcount() != sizeof(out)) {
		throw std::logic_error("������ �� ���� ����-����\n");
	}
	return out;
}

void encrypt::XTEA::run()
{
	try
	{
		switch (_mode)
		{
		case encrypt::XTEA::XTEAmode::ENC:
		case encrypt::XTEA::XTEAmode::DEC: {
			char r[8]{};//������������� ������ ����������
			uint32_t blocks[2]{};
			for (size_t i = 0; i < _maxsize; i += 8) {
				//����
				this->_if->read(
					r,
					(i + 8 > _maxsize ? _maxsize - i : 8)
				);
				// WARNING ������ ����������� � �������� �������, 
				// �� ���� ��������� ��� ������������ �������� xtea �� ������������������
				// plain > enc > dec > plain �����������
				memcpy(blocks, r, 8);

				//���������
				if (_mode == XTEAmode::ENC) {
					_encode(blocks);
				} else {
					_decode(blocks);
				}

				memcpy(r, blocks, 8);

				//�����
				this->_of->write(r, 8);

				//���������
				blocks[0] = 0;
				blocks[1] = 0;
			}
		}
			break;
		case encrypt::XTEA::XTEAmode::GEN:
			this->gen(this->_of);
			break;
		default:
			break;
		}
	} catch (const std::exception& e)
	{
		this->_ef->write((char*)e.what(), strlen(e.what()));
		_mode = XTEAmode::README;
		this->readme();
	}
}

void encrypt::XTEA::readme()
{
	const std::string s =
		"�������������:\n"
		"encrypter xtea {�����} [�����]: \n"
		"\n"
		"������:\n"
		" gen    ������������� ����-���� ��� ������������ ����������.\n" 
		"        ����� - of �����������\n"
		" enc    ����������� ����. ���� ����� -kf �� �������, �� �����\n"
		"        ������������ ����-���� � ��� �� ���������� ��� � -of.\n"
		"        ������������� ��� ����� -kf � -of ������������ �����������\n"
		" dec    ������������ ����. ���� ����� -kf �� �������, �� �����\n"
		"        ������� ������ ���� ����� � ��� �� ���������� ��� �\n"
		"        -if, � ��� �� ������, �� � ����������� .key. �������������\n"
		"        ��� ����� -kf � -if �����������\n"
		"�����:\n"
		" -if \"/path/to/plain.txt\"   ���� ������ �� �����\n"
		" -of \"/path/to/encoded.txt\" ����� ������ � ����\n"
		" -kf \"/path/to/file.key\"    ������������� ����-����� �� ���������\n"
		"                            ����\n"
		" -nr 32                     ���������� ���� ���������� ������� ����� \n"
		"                            �������. �� ��������� ���������� ����� 32\n"
		" -size 2048                 ������������ ���������� ������ �����������\n"
		"                            ���������� / ������������. �� ������ ���������\n"
		"                            ������ �������� �����, ����� - ������������� \n"
		"                            ���������. ������������ ����� � ������� \n"
		"                            enc / dec\n"
		"�������:\n"
		" encrypter xtea gen -of \"/path/to/encoded.txt\"\n"
		" encrypter xtea enc -size 2048 -kf \"/path/to/file.key\" -if \"/path/to/plain.txt\" -of \"/path/to/encoded.txt\"\n"
		" encrypter xtea dec -size 2048 -kf \"/path/to/file.key\" -if \"/path/to/encoded.txt\" -of \"/path/to/decoded.txt\"\n"
		"��� �������������� ����������, �������� https://github.com/HtieDgh/encrypter\n";
	this->_ef->write((char*)s.c_str(), s.size());
}

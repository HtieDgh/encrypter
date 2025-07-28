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
	this->_mode = XTEAMode::README;
}

encrypt::XTEA::XTEA(char* modename, std::map<std::string, const char*>& params, OutputStrategy* const errout)
{
	this->setErrOutput(errout);
	try
	{
		// ВЫБОР РЕЖИМА
		if (!strcmp(modename, "enc"))
		{
			this->_mode = XTEAMode::ENC;
		} else if (!strcmp(modename, "dec"))
		{
			this->_mode = XTEAMode::DEC;
		} else if (!strcmp(modename, "gen"))
		{
			this->_mode = XTEAMode::GEN;
			if (params.count("-of") == 0) {
				throw encrypt::XTEA::XTEAMode(encrypt::XTEA::XTEAMode::NOOUTPUTGEN);
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
			throw encrypt::XTEA::XTEAMode(encrypt::XTEA::XTEAMode::README);
		} else {
			throw encrypt::XTEA::XTEAMode(encrypt::XTEA::XTEAMode::NOMODE);
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
			// По умолчанию ввод из stdin иначе ввод из файла
			this->setInput(new StdoutInput());
		} else {
			this->setInput(new FileInput(params["-if"]));
		}
		
		if (_mode != XTEAMode::GEN && params.count("-size") != 0) {
			this->_maxsize = atoi(params["-size"]);//atoi() переводит char* в int
		}

		if (params.count("-nr") != 0)
		{
			this->_nr = atoi(params["-nr"]);
		}
		// исключения при сочетании режимов и опций
		if (_mode == XTEAMode::ENC && params.count("-kf") == 0 && params.count("-of") == 0)
		{
			throw encrypt::XTEA::XTEAMode(encrypt::XTEA::XTEAMode::NOKFENC);			
		}
		if (_mode == XTEAMode::DEC && params.count("-kf") == 0 && params.count("-if") == 0)
		{
			throw encrypt::XTEA::XTEAMode(encrypt::XTEA::XTEAMode::NOKFDEC);
		}

		// без разницы это dec enc gen - _key должен быть заполнен перед выполнением!
		if (params.count("-kf") != 0 && _mode != XTEAMode::GEN)
		{
			_key = reedKeyFile(params["-kf"]);
		} else if (
			params.count("-kf") == 0 &&
			_mode == XTEAMode::ENC
			) {
			// сгенерировать ключ-файл в той же директории что и результат шифрования (-of)
			std::string s = params["-of"]; s += _KEYFILEEXT;
			FileOutput* okf = new FileOutput(s.c_str());
			this->gen(okf);
			delete okf;
		} else if (
			params.count("-kf") == 0 &&
			_mode == XTEAMode::DEC
			) {
			// попытка прочитать файл в той же директории что и -if
			std::string s = params["-if"]; s += _KEYFILEEXT;
			_key = reedKeyFile(s.c_str());
		}
	} catch (const encrypt::XTEA::XTEAMode& e)
	{
		this->mode(e);
	}
}

encrypt::XTEA::~XTEA()
{
	delete[] this->_key;
}

void encrypt::XTEA::gen(OutputStrategy* okf)
{
	_gen();
	char out = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 3; j >= 0; j--) {
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
		throw encrypt::XTEA::XTEAMode(encrypt::XTEA::XTEAMode::NOACCESKF);
	}

	fin.read(reinterpret_cast<char*>(out), 16);

	if (fin.gcount() != 16) {
		throw encrypt::XTEA::XTEAMode(encrypt::XTEA::XTEAMode::NOFULLKF);
	}
	return out;
}

void encrypt::XTEA::run()
{
	_done = false;
	switch (_mode)
	{
	case encrypt::XTEA::XTEAMode::ENC:
	case encrypt::XTEA::XTEAMode::DEC: {
		try
		{
			char r[8]{};//инициализация нулями необходима
			uint32_t blocks[2]{};
			for (size_t i = 0; !_done; i += 8) {
				//ВВОД
				if( 
					this->_if->read(r,8) != 8 ||
					_maxsize > 0 && i + 8 >= _maxsize
				) {
					//если прочитано меньше 8 байт, то файл закончился, команду можно завершать
					_done = true;
				}

				// WARNING данные считываются в обратном порядке, 
				// не знаю насколько это классический алгоритм xtea но последовательность
				// plain > enc > dec > plain выполняется
				memcpy(blocks, r, 8);

				//ОБРАБОТКА
				if (_mode == XTEAMode::ENC) {
					_encode(blocks);
				} else {
					_decode(blocks);
				}

				memcpy(r, blocks, 8);

				//ВЫВОД

				this->_of->write( 
					r, 
					_mode == encrypt::XTEA::XTEAMode::DEC && _done ? _maxsize-i : 8
				);
			}
		} catch (const std::exception& e)
		{
			this->_ef->write((char*)e.what(), strlen(e.what()));
			_mode = XTEAMode::README;
			this->readme();
		}
	}
		break;
	case encrypt::XTEA::XTEAMode::GEN:
		this->gen(this->_of);
		break;
	case encrypt::XTEA::XTEAMode::NOOUTPUTGEN:
		this->_ef->write((char*)("Команда gen требует обязательного использования опции -of\n"), 58);//TR10
		this->readme();
		break;
	case encrypt::XTEA::XTEAMode::README:
		this->_ef->write((char*)("Вывод справки по использованию:\n"),32);//TR11
		this->readme();
		break;
	case encrypt::XTEA::XTEAMode::NOMODE:
		this->_ef->write((char*)("Такой команды не существует\n"),28);//TR12
		this->readme();
		break;
	case encrypt::XTEA::XTEAMode::NOKFENC:
		this->_ef->write((char*)("Невозможно использование команды enc без опции -kf и -of\n"),57);//TR14// can't use enc without -kf and -of\n
		this->readme();
		break;
	case encrypt::XTEA::XTEAMode::NOKFDEC:
		this->_ef->write((char*)("Невозможно использование команды dec без опции -kf и -if\n"),57);//TR15//can't use dec without -kf and -if\n
		this->readme();
		break;
	case encrypt::XTEA::XTEAMode::NOACCESKF:
		this->_ef->write((char*)("Файл-ключ не найден или к нему нет доступа\n"),43);//TR16
		break;
	case encrypt::XTEA::XTEAMode::NOFULLKF:
		this->_ef->write((char*)("Считан не весь ключ-файл\n"),25);//TR17
		break;
	}

}

void encrypt::XTEA::readme()
{//TR18
	const std::string s =
		"Использование:\n"
		" encrypter xtea {режим} [опции]\n"
		"\n"
		"Режимы:\n"
		" gen    Сгенерировать ключ-файл для последующего шифрования.\n" 
		"        Опция - of обязательна\n"
		" enc    Зашифровать файл. Если опция -kf не указана, то будет\n"
		"        сгенерирован ключ-файл в той же директории что и -of.\n"
		"        Использование без опций -kf и -of одновременно недопустимо\n"
		" dec    Расшифровать файл. Если опция -kf не указана, то будет\n"
		"        попытка поиска ключ файла в той же директории что и\n"
		"        -if, с тем же именем, но с расширением .key. Использование\n"
		"        без опции -kf и -if недопустимо\n"
		"Опции:\n"
		" -if \"/path/to/plain.txt\"   Ввод данных из файла\n"
		" -of \"/path/to/encoded.txt\" Вывод данных в файл\n"
		" -kf \"/path/to/file.key\"    Использование ключ-файла по заданному\n"
		"                            пути\n"
		" -nr 32                     Определить иное количество раундов петли \n"
		"                            Фестеля. По умолчанию количество равно 32\n"
		" -size 2048                 Ограничивает количество байтов необходимых\n"
		"                            шифрования / дешифрования. Если превысит размер\n" 
		"                            исходного файла, размер выходного файла будет не\n"
		"                            превышать исходный более чем на 1 блок\n"
		"Примеры:\n"
		" encrypter xtea gen -of \"/path/to/encoded.txt\"\n"
		" encrypter xtea enc -size 2048 -kf \"/path/to/file.key\" -if \"/path/to/plain.txt\" -of \"/path/to/encoded.txt\"\n"
		" encrypter xtea dec -kf \"/path/to/file.key\" -if \"/path/to/encoded.txt\" -of \"/path/to/decoded.txt\"\n"
		"Для дополнительной информации, посетите https://github.com/HtieDgh/encrypter\n";
	this->_ef->write((char*)s.c_str(), s.size());
}

encrypt::XTEA::XTEAMode encrypt::XTEA::mode() const
{
	return _mode;
}

void encrypt::XTEA::mode(XTEAMode mode)
{
	this->_mode = mode;
}

void encrypt::XTEA::setKey(uint32_t*& key)
{
	if (_key) {
		delete this->_key;
	}
	_key = key;
}

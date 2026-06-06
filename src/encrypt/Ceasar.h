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
#pragma once
#include"Algorithm.h"
#include<map>

namespace encrypt {
	class Ceasar : public Algorithm
	{
	private:
		size_t _keysize=0,	//Размер ключа, так как ключ может содержать null символы влияющие на работу strlen()
			   _kd=0;			//Смещение ключа, используются в run()
		size_t _d=0,			//Счетчик итераций чтения с потока, используется в run()
			   _maxsize=0;	//Макс размер в байтах после которого следует остановить шифрование
		char* _key=nullptr;		//Указатель на ключ-строку
		char _dec=0;			//Промежуточный результат шифрования 1 байта
		InputStrategy* _keyFile=nullptr;
	public:
		
		enum class CeasarMode
		{
			ENC=5,
			DEC=4,
			NOMODE=1,
			NOKEY=3,
			README=2
		};
		Ceasar(std::wstring modename, std::map<std::wstring, std::wstring>& params, OutputStrategy* errout);

		char* key() const;
		void key(char* _key);
		size_t maxsize() const;
		void maxsize(size_t maxsize);
		
		void setKeyFile(InputStrategy* kf);
		void run() override;
		CeasarMode mode() const;
		void mode(CeasarMode mode);
		void readme() override;
		
		~Ceasar();
	private:
		char _nextKey();
		CeasarMode _mode;			//Текущий режим работы
	};
}
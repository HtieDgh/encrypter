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
		int _kd;			//Смещение ключа, используются в run()
		const char* _key;	//Указатель на ключ-строку
		char _dec;			//Промежуточный результат шифрования 1 байта
		size_t _d,			//Счетчик итераций чтения с потока, используется в run()
			_maxsize;		//Макс размер в байтах после которого следует остановить шифрование
	public:
		enum class CeasarMode
		{
			ENC,
			DEC,
			NOMODE,
			NOKEY,
			README
		};
		Ceasar();
		Ceasar(char* modename, std::map<std::string, const char*>& params,OutputStrategy* const errout);

		const char* key() const;
		size_t maxsize() const;
		void key(const char* _key);
		void maxsize(size_t maxsize);

		void run() override;
		CeasarMode mode() const;
		void mode(CeasarMode mode);
		void readme() override;
	private:
		CeasarMode _mode;			//true если требуется расшифровать
		
	/*	class CesarBuilder {
			private:
				bool _mode;			
				int _kd;			
				const char* _key;	
				char _dec;			
				size_t _d,			
					_maxsize;		
				bool _ready = false;
			public:
				CesarBuilder();
			
				CesarBuilder& modename(char* modename);
				CesarBuilder& key(const char* _key);
				CesarBuilder& key(const char* _key);

				Ceasar* build();
		};*/
	};
}
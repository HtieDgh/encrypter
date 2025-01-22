#pragma once
#include"Algoritm.h"
namespace encrypt {
	class Ceasar : public Algorithm
	{
	private:
		bool _done;		//Статус шифрования
		bool _mode;		//true если требуется расшифровать
		int _d, _kd;	//смещение данных и ключа
		const char* _key;		//Указатель на ключ
		char _dec;		//Промежуточный результат шифрования 1 байта
		size_t _maxsize;//Макс размер в байтах после которого следует остановить шифрование
	public:
		Ceasar();
		Ceasar(char* _key, size_t maxsize);
		~Ceasar();

		const char* key() const;
		size_t maxsize() const;
		void key(const char* _key);
		void maxsize(int maxsize);

		void run() override;
		bool mode() const override;
		void mode(bool isEnc) override;
	};
}
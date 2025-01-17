#pragma once
#include"Algoritm.h"
namespace encrypt {
	class Ceaser : public Algoritm
	{
	private:
		bool _done;		//Статус шифрования
		bool _mode;		//true если требуется расшифровать
		int _d, _kd;	//смещение данных и ключа
		char* _key;		//Указатель на ключ
		char _dec;		//Промежуточный результат шифрования 1 байта
		char _enc;		//Байт готовый к шифрованию
		int _maxsize;//Макс размер в байтах после которого следует остановить шифрование
	public:
		Ceaser();
		Ceaser(char* _key, size_t maxsize);
		~Ceaser();

		char* key() const;
		size_t maxsize() const;
		void key(char* _key);
		void maxsize(int maxsize);

		void run() override;
		bool mode() const override;
		void mode(bool isEnc) override;
	};
}
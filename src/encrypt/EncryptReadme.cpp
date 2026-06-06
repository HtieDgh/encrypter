#include "EncryptReadme.h"
#include "Translator.h"
using T = encrypt::Translator;

void encrypt::EncryptReadme::run()
{
	switch (head)
	{
	case encrypt::EncryptReadme::PRESTATE::noalg:
		this->_ef->write(T::i()->msg({L"encrypter",3}));
		break;
	default:
		break;
	}
	this->_ef->write(T::i()->msg({L"encrypter",1}));
}

void encrypt::EncryptReadme::readme()
{
	this->_ef->write(T::i()->msg({L"encrypter",2}));
}

encrypt::EncryptReadme::EncryptReadme(StderrOutput* e, PRESTATE head):
	head{head}
{
	this->setErrOutput(e);
}
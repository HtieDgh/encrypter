#include "EncryptReadme.h"
#include "Translator.h"
using T = encrypt::Translator;
using MODE = encrypt::EncryptReadme::PRESTATE;
void encrypt::EncryptReadme::run()
{
	switch (head)
	{
	case MODE::noalg:
		this->_ef->write(T::msg({L"encrypter",3}));
		break;
	default:
		break;
	}
	this->_ef->write(T::msg({L"encrypter",1}));
}

void encrypt::EncryptReadme::readme()
{
	this->_ef->write(T::msg({L"encrypter",2}));
}

encrypt::EncryptReadme::EncryptReadme(StderrOutput* e, PRESTATE head):
	head{head}
{
	this->setErrOutput(e);
}
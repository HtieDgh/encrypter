#include "Algoritm.h"

void encrypt::Algorithm::setOutput(OutputStrategy* s)
{
	this->_of= s;
}

void encrypt::Algorithm::setInput(InputStrategy * s)
{
	this->_if= s;
}

encrypt::Algorithm::~Algorithm()
{
	if (this->_of)
	{
		delete this->_of;
	}
	if (this->_if)
	{
		delete this->_if;
	}
}

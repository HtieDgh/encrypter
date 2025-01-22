#include "Algoritm.h"

void encrypt::Algorithm::setOutput(OutputStrategy* s)
{
	this->_view = s;
}

encrypt::Algorithm::~Algorithm()
{
	delete this->_view;
}

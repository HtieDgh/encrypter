#include "SetAlgoritm.h"

void encrypt::SetAlgoritm::setAlgoritm(Algoritm* alg)
{
	this->_alg = alg;
}

void encrypt::SetAlgoritm::doAlgoritm()
{
	_alg->run();
}
encrypt::SetAlgoritm::~SetAlgoritm()
{
	delete _alg;
};

#include "AlgorithmStrategy.h"

void encrypt::AlgorithmStrategy::setAlgorithm(Algorithm* alg)
{
	this->_alg = alg;
}

void encrypt::AlgorithmStrategy::doAlgorithm()
{
	_alg->run();
}
encrypt::AlgorithmStrategy::~AlgorithmStrategy()
{
	delete _alg;
};

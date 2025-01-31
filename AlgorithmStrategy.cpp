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
#include "AlgorithmStrategy.h"
#include<sstream>
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
	if(_alg) delete _alg;
};
void encrypt::AlgorithmStrategy::readme(OutputStrategy* out)
{
	std::stringstream s;
	s << "Использование:\n"
		<< "\n"
		<< "encrypter {имя алгоритма} {команда} [опции] -if \\path\\to\\input.txt -of \\path\\to\\output.txt\n"
		<< "\n"
		<< "Примечание:\n"
		<< "\t{имя алгоритма} например ceasar\n"
		<< "\t{команда} и ее [опции] зависят от алгоритма, попробуйте изучить, например:\n"
		<< "\n"
		<< "\tencrypter ceasar -?";
	  
	out->write((char*)s.str().c_str(), s.str().size());
}
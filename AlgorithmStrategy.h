#pragma once
#include "Algoritm.h"
namespace encrypt {
	class AlgorithmStrategy		
	{
	private:
		Algorithm* _alg = nullptr;
	public:
		AlgorithmStrategy()=default;
		void setAlgorithm(Algorithm*);
		void doAlgorithm();
		~AlgorithmStrategy();
	};
}



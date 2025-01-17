#pragma once
#include "Algoritm.h"
namespace encrypt {
	class SetAlgoritm		
	{
	private:
		Algoritm* _alg;
	public:
		SetAlgoritm()=default;
		void setAlgoritm(Algoritm*);
		void doAlgoritm();
		~SetAlgoritm();
	};
}



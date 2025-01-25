#pragma once
#include"OutputStrategy.h"
#include"InputStrategy.h"
namespace encrypt {
	class Algorithm
	{
	protected:
		OutputStrategy* _of = nullptr;
		InputStrategy* _if=nullptr;
	public:								   
		virtual bool mode() const = 0;
		virtual void mode(bool isEnc)=0;
		virtual void run()=0;

		void setOutput(OutputStrategy*);
		void setInput(InputStrategy*);
		~Algorithm();
	};
}



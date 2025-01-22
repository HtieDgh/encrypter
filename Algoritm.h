#pragma once
#include"OutputStrategy.h"
namespace encrypt {
	class Algorithm
	{
	protected:
		OutputStrategy* _view = nullptr;
	public:
		virtual bool mode() const = 0;
		virtual void mode(bool isEnc)=0;
		virtual void run()=0;

		void setOutput(OutputStrategy*);
		~Algorithm();
	};
}



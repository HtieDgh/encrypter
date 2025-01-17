#pragma once
namespace encrypt {
	class Algoritm
	{
	public:
		virtual bool mode() const = 0;
		virtual void mode(bool isEnc)=0;
		virtual void run()=0;

	};
}



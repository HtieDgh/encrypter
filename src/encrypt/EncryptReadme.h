#pragma once
#include "Algorithm.h"
namespace encrypt{
	class EncryptReadme : public Algorithm
	{
	public:
		enum class PRESTATE
		{
			none,
			noalg
		};
		// Унаследовано через Algorithm
		void run() override;
		void readme() override;
		EncryptReadme(StderrOutput* e, PRESTATE head = PRESTATE::none);
	private:
		PRESTATE head;
	};
}



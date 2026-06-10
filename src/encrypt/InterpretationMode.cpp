#include "InterpretationMode.h"
#include "Translator.h"
#include "Ceasar.h"
#include "XTEA.h"
#include "EncrypterConfig.h"
#include "KeystreamGenerator.h"
#include "General.h"

using T = encrypt::Translator;
using G = encrypt::General;

void encrypt::InterpretationMode::run()
{
	std::map<std::wstring, std::wstring> params;

	while (true) {
		if (alg_) delete alg_;
		std::vector<std::wstring> vec;
		params = G::buildParams(std::wcin, vec);
		if (params.count(L"?") != 0) {//вывод справки по Интерпритируемому режиму
			this->readme();
			continue;
		}
		if (params.count(L"!") != 0) {//получен сигнал о завершении программы
			break;
		}


		if (params.count(L"ceasar") != 0)		// АГЛОРИТМ ЦЕЗАРЯ
		{
			this->alg_ = new Ceasar(vec[1], params, this->_of);

		} else if (params.count(L"xtea") != 0)	// XTEA
		{
			this->alg_ = new XTEA(vec[1], params, this->_of);

		} else if (params.count(L"chacha") != 0) // chacha20 KeystreamGenerator
		{
			this->alg_ = new KeystreamGenerator(vec[1], params, this->_of);
		} else {
			//algname не определено, вывод справки
			this->readme();
		}
	}
}

void encrypt::InterpretationMode::readme()
{
	this->_ef->write(T::msg({L"interpritationmode",1}));
}

encrypt::InterpretationMode::InterpretationMode(std::wstring modename,InputStrategy* cmdsource, OutputStrategy* cmdoutput, OutputStrategy* modeoutput) noexcept
{
	this->setInput(cmdsource);
	this->setOutput(cmdoutput);
	this->setErrOutput(modeoutput);

}

encrypt::InterpretationMode::~InterpretationMode()
{
	if (alg_) delete alg_;
}


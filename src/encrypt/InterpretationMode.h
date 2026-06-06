#pragma once
#include "Algorithm.h"
namespace encrypt {

    class InterpretationMode : public Algorithm
    {
    private:
        Algorithm* alg_;

        std::wstring algname_;
    public:
        // Унаследовано через Algorithm
        void run() override;
        void readme() override;

        InterpretationMode(std::wstring modename,InputStrategy* cmdsource, OutputStrategy* cmdoutput, OutputStrategy* modeoutput) noexcept;
        ~InterpretationMode();
    };

}


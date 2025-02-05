#pragma once
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
#include "Algorithm.h"
#include<map>

namespace encrypt {
    class XTEA : public Algorithm
    {
    private:
        enum class XTEAmode
        {
            ENC,
            DEC,
            GEN,
            README
        } _mode;                // Различные режимы работы алгоритма
        uint32_t* _key = nullptr;
        size_t _maxsize = -1;	    // Макс размер в байтах после которого следует остановить шифрование
        uint32_t _nr = 32;          // Количество раундов Фестеля
        const char _KEYFILEEXT[32] = ".key";
        
        void _encode(uint32_t* v);
        void _decode(uint32_t* v);

        uint32_t* _gen();
    public:
        XTEA();
        XTEA(char* modename, std::map<std::string, const char*>& const params, OutputStrategy* const errout);
        ~XTEA();

        void gen(OutputStrategy* okf);
        
        uint32_t* reedKeyFile(const char* path);

        void run() override;
        void readme() override;
    };
}


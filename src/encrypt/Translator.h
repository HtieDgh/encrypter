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
#pragma once
#include <string>
#include <map>
#include"InputStrategy.h"
#include "ConfigParser.h"

namespace encrypt
{
    class TranslatorParser
    {
    protected:
        std::wstring _trim(const std::wstring& str);
    public:
        std::map<std::pair<std::wstring, size_t>, std::wstring> extract(InputStrategy* _if);
    };

    class Translator {
    private:       
        static Translator* _instance;
        InputStrategy* _localeSource; // Источник строк-сообщений
        TranslatorParser* _parser;           // Парсер источника строк-сообщений. Используется в getMsg        static Translator* _instance;

        //{Имя алгоритма, id сообщения}=>L"Данные"
        std::map<std::pair<std::wstring,size_t>, std::wstring> _msgs;

        Translator(InputStrategy* localeSource, 
                   TranslatorParser* parser
        );
    public:
        ~Translator();
        static Translator* i(
            InputStrategy* lcs = nullptr,
            TranslatorParser* p = nullptr
        );
        void setTranslatorParser(TranslatorParser* p);
        void setLocaleSource(InputStrategy* lcs);
        
        //Получить данные перевода по [key] = {Имя алгоритма, id сообщения}
        std::wstring getMsg(std::pair<std::wstring, size_t> key);

    };
}
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
#include "Translator.h"
#include<sstream>

encrypt::Translator* encrypt::Translator::_instance = nullptr;

encrypt::Translator::Translator(InputStrategy* localeSource, TranslatorParser* parser)
    : _localeSource(localeSource),
    _parser(parser)
{
    this->_msgs = this->_parser->extract(this->_localeSource);
}

encrypt::Translator::~Translator()
{
    if (this->_localeSource) {
        delete this->_localeSource;
    }
    if (this->_parser) {
        delete this->_parser;
    }
    encrypt::Translator::_instance=nullptr;
}

encrypt::Translator* encrypt::Translator::i(InputStrategy* lcs, TranslatorParser* p)
{
    if (encrypt::Translator::_instance == nullptr) {
        encrypt::Translator::_instance = new 
            Translator(lcs,p);
    }
    return encrypt::Translator::_instance;
}

void encrypt::Translator::setTranslatorParser(TranslatorParser* p)
{
    if (this->_parser) {
        delete this->_parser;
    }
    this->_parser = p;
}

void encrypt::Translator::setLocaleSource(InputStrategy* lcs)
{
    if (this->_localeSource) {
        delete this->_localeSource;
    }
    this->_localeSource = lcs;
}

std::wstring encrypt::Translator::getMsg(std::pair<std::wstring, size_t> key)
{
    //Первый запуск 
    if (_msgs.count(key) == 0) {
        return L"NOT-TRANSLATED-YET";
    }
    return _msgs[key];
}

//====================================================================================

std::wstring encrypt::TranslatorParser::_trim(const std::wstring& str)
{
    size_t first = str.find_first_not_of(L" \t\r");
    if (std::wstring::npos == first) {
        return L"";
    }
    size_t last = str.find_last_not_of(L" \t\r");
    return str.substr(first, (last - first + 1));
}

std::map<std::pair<std::wstring, size_t>, std::wstring> encrypt::TranslatorParser::extract(InputStrategy* _if)
{
    std::wstring line;
    std::map<std::pair<std::wstring, size_t>, std::wstring> _config;
    std::wstring alg{};
    std::wstring value{};
    size_t delimiter_pos = 0;
    size_t id{};
    size_t first{};
    size_t last{};
    enum {NOALG,START,READ,ALG,MSG,MSGEND,ERROR,STOP} S = START;

    try {
        while (true) {
            switch (S)
            {
            case START:
                if (!_if->readln(line)) throw STOP;
                line = TranslatorParser::_trim(line);
                if (line[0] == L'#' || line.empty()) continue;
                S = NOALG;
                break;

            case NOALG:
                first = line.find(L'[') + 1;
                last = line.find_last_not_of(L']');
                if (first == std::wstring::npos || last == std::wstring::npos)
                {
                    S = ERROR;
                    continue;
                }
                alg = line.substr(first, (last - first + 1));
                S = ALG;
                break;

            case ALG:
                if(!_if->readln(line)) throw STOP;
                line = TranslatorParser::_trim(line);
                if (line[0] == L'#' || line.empty()) continue;

                delimiter_pos = line.find(L'=');
                if (delimiter_pos == std::wstring::npos) {
                    if (line[0] == L'[') {
                        S = NOALG;
                        continue;
                    } else {
                        S = ERROR;
                        continue;
                    }
                }
                try//out of range exeption [id]
                {
                    id = std::stoull(line.substr(0, delimiter_pos));
                    value += line.substr(delimiter_pos + 1);
                    //Строка не закончена, продолжение чтения построчно
                    if (value[value.size() - 1] != L'$')
                    {
                        S = MSG;
                        continue;
                    } else {
                        S = MSGEND;
                    }
                } catch (...)
                {
                    continue;
                }
                break;

            case MSG:
                _if->readln(line);
                // Продолжение заполнения строки
                value += L'\n' + line;
                if (value.find(L'$') != std::wstring::npos)
                {
                    // Достигнут конец msg - сохранение msg
                    S = MSGEND;
                }
                break;

            case MSGEND:
                // Сохранение msg
                value = value.substr(0, value.find(L'$'));
                _config[{alg, id}] = value;
                value = L"";
                S = ALG;
                break;

            case ERROR:
                throw ERROR;
                break;

            }
        }
    } catch (...) {}
    
    return _config;
}

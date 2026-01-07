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
#include "EncrypterConfig.h"
#include "Translator.h"
using T = encrypt::Translator;

encrypt::EncrypterConfigParser::EncrypterConfigParser(encrypt::InputStrategy* source) noexcept
{
    std::wstring line;
    while (source->readln(line)) {
        // Пропуск пустых строк и комментариев
        line = ConfigParser::_trim(line);
        if (line.empty() || line[0] == L'#') {
            continue;
        }

        // Разделение строки на ключ и значение
        size_t delimiter_pos = line.find(L'=');
        if (delimiter_pos == std::wstring::npos) {
            continue; // Пропуск строки без разделителя
        }

        std::wstring key = _trim(line.substr(0, delimiter_pos));
        std::wstring value = _trim(line.substr(delimiter_pos + 1));

        if (!key.empty()) {
            _config[key] = value;
        }
    }
}

std::wstring encrypt::EncrypterConfigParser::getWstring(const std::wstring& key, const std::wstring& defaultValue) const
{
    auto it = _config.find(key);
    if (it != _config.end()) {
        return it->second;
    }
    return defaultValue;
}

int encrypt::EncrypterConfigParser::getInt(const std::wstring& key, int defaultValue) const
{
    auto it = _config.find(key);
    if (it != _config.end()) {
        try {
            return std::stoi(it->second);
        } catch (...) {
            return defaultValue;
        }
    }
    return defaultValue;
}

double encrypt::EncrypterConfigParser::getDouble(const std::wstring& key, double defaultValue) const
{
    auto it = _config.find(key);
    if (it != _config.end()) {
        try {
            return std::stod(it->second);
        } catch (...) {
            return defaultValue;
        }
    }
    return defaultValue;
}

bool encrypt::EncrypterConfigParser::getBool(const std::wstring& key, bool defaultValue) const
{
    auto it = _config.find(key);
    if (it != _config.end()) {
        std::wstring value = it->second;
        std::transform(value.begin(), value.end(), value.begin(), ::tolower);

        if (value == L"true" || value == L"yes" || value == L"1") {
            return true;
        } else if (value == L"false" || value == L"no" || value == L"0") {
            return false;
        }
    }
    return defaultValue;
}

bool encrypt::EncrypterConfigParser::empty()
{
    return _config.empty();
}

std::map<std::wstring, std::wstring> encrypt::EncrypterConfigParser::getAll()
{
    return this->_config;
}

encrypt::EncrypterConfig::EncrypterConfig(
    char* modename,
    std::map<std::string, const char*>& params,
    OutputStrategy* errout,
    std::wstring cfgSave,
    EncrypterConfigParser* configParser
)
    : _mode(encrypt::EncrypterConfig::ConfigMode::README)
{
    this->setErrOutput(errout);
    this->_cfgSave = cfgSave;
    this->setConfigParser(configParser);
    try {
        if (!strcmp(modename, "r") || !strcmp(modename, "read"))
        {
            mode(encrypt::EncrypterConfig::ConfigMode::READ);
        }
        else if (!strcmp(modename, "u") || !strcmp(modename, "update"))
        {
            mode(encrypt::EncrypterConfig::ConfigMode::UPDATE);
        }
        else if (!strcmp(modename, "c") || !strcmp(modename, "create"))
        {
            mode(encrypt::EncrypterConfig::ConfigMode::CREATE);
        }
        else if (
            !strcmp(modename, "-?") ||
            !strcmp(modename, "-help")
            )
        {
            throw encrypt::EncrypterConfig::ConfigMode::README;
        } else {
            throw encrypt::EncrypterConfig::ConfigMode::NOMODE;
        }
        
        // Установка прочитанного конфига
        this->_config = std::move(_configParser->getAll());

        // Если не create то установить значения конфига по умолчанию
        if (this->_mode != encrypt::EncrypterConfig::ConfigMode::CREATE && _configParser->empty())
        {
            this->_ef->write(T::i()->getMsg({L"encrypterconfig",3}));
            this->setString(L"locale", L"ru-RU");
        }

        // Изменение/создание параметров
        if (
            this->_mode == encrypt::EncrypterConfig::ConfigMode::CREATE ||
            this->_mode == encrypt::EncrypterConfig::ConfigMode::UPDATE
            ) {
            if (params.count("-locale"))
            {
                const char* cc{params["-locale"]};//TODO danger casting
                this->setString(L"locale", std::wstring(cc, cc + strlen(cc)));
            }
        }
        
    } catch (const encrypt::EncrypterConfig::ConfigMode& e) {
        this->mode(e);
    }
}

encrypt::EncrypterConfig::~EncrypterConfig()
{
    if (_configParser) {
        delete _configParser;
    }
}


void encrypt::EncrypterConfig::readme()
{
    this->_ef->write(T::i()->getMsg({L"encrypterconfig",4}));
}


encrypt::EncrypterConfig::ConfigMode encrypt::EncrypterConfig::mode() const
{
    return this->_mode;
}


void encrypt::EncrypterConfig::mode(ConfigMode mode)
{
    this->_mode = mode;
}


void encrypt::EncrypterConfig::setString(const std::wstring& key, const std::wstring& value)
{
    this->_config[key] = value;
}


void encrypt::EncrypterConfig::setConfigParser(EncrypterConfigParser* p)
{
    if (this->_configParser) {
        delete this->_configParser;
    }
    this->_configParser = p;
}


void encrypt::EncrypterConfig::save(OutputStrategy* o)
{
    for(const auto& [key, value] : _config)
    {
        o->write(key);
        o->write((wchar_t*)L"=", 1);
        o->write(value);
        o->write((wchar_t*)L"\n", 1);
    }
}


void encrypt::EncrypterConfig::run()
{
    switch (_mode)
    {
    case encrypt::EncrypterConfig::ConfigMode::READ:
        //const auto& [key, value] - структурное связывание С++17
        for (const auto& [key, value] : _config) {
            this->_ef->write(key);
            this->_ef->write((wchar_t*)L"=", 1);
            this->_ef->write(value);
            this->_ef->write((wchar_t*)L"\n",1);
        }
        break;
    case encrypt::EncrypterConfig::ConfigMode::CREATE:
    case encrypt::EncrypterConfig::ConfigMode::UPDATE:
        {
            encrypt::FileOutput* o = new FileOutput(this->_cfgSave.c_str());
            this->save(o);
            delete o;
        }
        break;
    case encrypt::EncrypterConfig::ConfigMode::README:
        this->_ef->write(T::i()->getMsg({L"encrypterconfig",1}));
        this->readme();
        break;
    case encrypt::EncrypterConfig::ConfigMode::NOMODE:
        this->_ef->write(T::i()->getMsg({L"encrypterconfig",2}));
        this->readme();
        break;
    default:
        break;
    }
}
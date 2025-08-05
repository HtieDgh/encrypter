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
#include<map>
#include<string>
#include<iostream>
#include<fstream>
#include"OutputStrategy.h"
#include"Algorithm.h"
#include"ConfigParser.h"

namespace encrypt {
    class EncrypterConfigParser : public ConfigParser
    {
    protected:
        std::map<std::wstring, std::wstring> _config;

    public:
        // Загрузка конфигурации из файла
        EncrypterConfigParser(InputStrategy* source) noexcept;

        // Получение строкового значения
        std::wstring getWstring(const std::wstring& key, const std::wstring& defaultValue = L"") const override;

        // Получение целочисленного значения
        int getInt(const std::wstring& key, int defaultValue = 0) const override;

        // Получение значения с плавающей точкой
        double getDouble(const std::wstring& key, double defaultValue = 0.0) const override;

        // Получение булевого значения
        bool getBool(const std::wstring& key, bool defaultValue = false) const override;

        // Проверка на пустоту
        bool empty();

        // Возврат всего конфига
        std::map<std::wstring, std::wstring> getAll();
    };

    class EncrypterConfig : public Algorithm
    {
    public:
        enum class ConfigMode
        {
            CREATE,
            READ,
            UPDATE,
            NOMODE,
            README
        };
        EncrypterConfig(
            char* modename,
            std::map<std::string, const char*>& params, 
            OutputStrategy* errout,
            std::wstring cfgSave,
            EncrypterConfigParser* configParser
        );
        ~EncrypterConfig();

        // Изменение параметра конфигурации
        void setString(const std::wstring& key, const std::wstring& value);
        void setConfigParser(EncrypterConfigParser* p);
        void save(OutputStrategy* o);

        void run() override;
        void readme() override;
        ConfigMode mode() const;
        void mode(ConfigMode mode);
    private:
        ConfigMode _mode;
        std::wstring _cfgSave;
        EncrypterConfigParser* _configParser;
        std::map<std::wstring, std::wstring> _config;
    };
}
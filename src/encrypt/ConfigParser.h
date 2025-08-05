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
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include "InputStrategy.h"
/// <summary>
/// Поддерживает конфиги в формате
///    <ключ>=<значение> 
///    с комментариями начинающиеся с символа '#'
/// </summary>
namespace encrypt
{
    class ConfigParser
    {
    protected:
        static std::wstring _trim(const std::wstring& str);

    public:
        // Получение строкового значения
        virtual std::wstring getWstring(const std::wstring& key, const std::wstring& defaultValue = L"") const = 0;

        // Получение целочисленного значения
        virtual int getInt(const std::wstring& key, int defaultValue = 0) const = 0;

        // Получение значения с плавающей точкой
        virtual double getDouble(const std::wstring& key, double defaultValue = 0.0) const = 0;

        // Получение булевого значения
        virtual bool getBool(const std::wstring& key, bool defaultValue = false) const = 0;
    };
}

/*
   Copyright 2026 Htie digital

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
#include "KeystreamGenerator.h"
#include "Translator.h"
#include <algorithm>
using MODE = encrypt::KeystreamGenerator::KeystreamGeneratorMode;
using T = encrypt::Translator;

encrypt::KeystreamGenerator::KeystreamGenerator(std::wstring modename, std::map<std::wstring, std::wstring>& params, OutputStrategy* const errout) {
    this->setErrOutput(errout);
    std::vector<uint8_t> iv{0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07};
    std::vector<uint8_t> key;
    try
    {
        // ВЫБОР РЕЖИМА
        if (modename == L"gen")
        {
            this->mode(MODE::GEN);
        } else if (
            modename == L"-?" ||
            modename == L"-help"
        ){
            throw MODE::README;
        }
        else
        {
            throw MODE::NOMODE;
        }

        // НАЗНАЧЕНИЕ ВВОДА
        if (params.count(L"-if") == 0)
        {
            // По умолчанию ввод из stdin иначе ввод из файла
            this->setInput(new StdInput());
        } else {
            this->setInput(new FileInput(params[L"-if"]));
        }

        // НАЗНАЧЕНИЕ ВЫВОДА
        if (params.count(L"-of") == 0)
        {
            //// По умолчанию вывод в stdout
            //this->setOutput(new StdOutput());
            throw MODE::NOOUTPUTGEN;
        } else {
            // Путь до файла назначен, вывод в этот файл
            this->setOutput(new FileOutput(ws2s(params[L"-of"].c_str()).c_str()));
        }

        //СКОЛЬКО ГЕНЕРИТЬ?
        if (params.count(L"-s") != 0 )
        {
            this->size_ = _wtoll(params[L"-s"].c_str());
        } else if (params.count(L"-size") != 0) {
            this->size_ = _wtoll(params[L"-size"].c_str());
        }
        else {
            this->size_ = KeystreamGenerator::DEFAULT_GEN_SIZE;
        }
        //ЧТЕНИЕ КЛЮЧ ФАЙЛА
        if (params.count(L"-kf") != 0) {
            key = reedKeyFile(ws2s(params[L"-kf"]).c_str());
        } else {
            throw MODE::NOKEYFILE;
        }

        // Инициализация состояния константами
        const uint32_t constants[4] = {
            0x61707865,  // "expa"
            0x3320646e,  // "nd 3"
            0x79622d32,  // "2-by"
            0x6b206574   // "te k"
        };
        std::memcpy(state_.data(), constants, 16);

        // Ключ (слова 4..11)
        std::memcpy(state_.data() + 4, key.data(), 32);

        // Счётчик (0) и IV записываются позже в методах инициализации
        // Обнуляем оставшиеся слова
        state_[12] = 0;
        state_[13] = 0;
        state_[14] = 0;
        state_[15] = 0;

        // Сохраняем IV для последующих сбросов
        iv_ = iv;
        key_ = key;

        reset();
    } catch (const MODE& m) {
        this->mode(m);
    }
}

void encrypt::KeystreamGenerator::reset() {
    counter_ = 0;
    pos_ = BLOCK_SIZE;   // при следующем запросе будет сгенерирован новый блок
    // Применяем сохранённые ключ и IV
    set_counter_and_iv(counter_, iv_);
}

void encrypt::KeystreamGenerator::generateBytes(uint8_t* buffer, size_t length) {
    size_t offset = 0;
    while (offset < length) {
        // Если текущий блок полностью использован, генерируем следующий
        if (pos_ == BLOCK_SIZE) {
            generate_next_block();
            pos_ = 0;
        }

        size_t to_copy = std::min(length - offset, BLOCK_SIZE - pos_);
        std::memcpy(buffer + offset, current_block_.data() + pos_, to_copy);
        offset += to_copy;
        pos_ += to_copy;
    }
}

void  encrypt::KeystreamGenerator::set_counter_and_iv(uint64_t counter, const std::vector<uint8_t>& iv) {
    // Счётчик (младшая и старшая половины)
    state_[12] = static_cast<uint32_t>(counter & 0xFFFFFFFF);
    state_[13] = static_cast<uint32_t>(counter >> 32);
    // IV (8 байт → два 32-битных слова)
    uint32_t iv0, iv1;
    std::memcpy(&iv0, iv.data(), 4);
    std::memcpy(&iv1, iv.data() + 4, 4);
    state_[14] = iv0;
    state_[15] = iv1;
}

// Генерация одного блока (ChaCha20 block function)
void encrypt::KeystreamGenerator::generate_next_block() {
    // Копируем текущее состояние (с установленными счётчиком и IV) для работы
    std::array<uint32_t,16> working = state_;

    // 20 раундов (10 двойных раундов)
    for (int round = 0; round < 10; ++round) {
        // Column rounds
        quarter_round(working[0], working[4], working[8], working[12]);
        quarter_round(working[1], working[5], working[9], working[13]);
        quarter_round(working[2], working[6], working[10], working[14]);
        quarter_round(working[3], working[7], working[11], working[15]);
        // Diagonal rounds
        quarter_round(working[0], working[5], working[10], working[15]);
        quarter_round(working[1], working[6], working[11], working[12]);
        quarter_round(working[2], working[7], working[8], working[13]);
        quarter_round(working[3], working[4], working[9], working[14]);
    }

    // Складываем с исходным состоянием
    for (int i = 0; i < 16; ++i)
        working[i] += state_[i];

    // Упаковываем полученные 16 слов в байтовый массив (little-endian)
    uint8_t* out = current_block_.data();
    for (int i = 0; i < 16; ++i) {
        uint32_t word = working[i];
        *out++ = word & 0xFF;
        *out++ = (word >> 8) & 0xFF;
        *out++ = (word >> 16) & 0xFF;
        *out++ = (word >> 24) & 0xFF;
    }

    // Увеличиваем счётчик на 1 для следующего блока
    ++counter_;
    set_counter_and_iv(counter_, iv_);
}

void encrypt::KeystreamGenerator::mode(KeystreamGeneratorMode v)
{
    this->mode_ = v;
}

void encrypt::KeystreamGenerator::state(std::array<uint32_t, 16> v)
{
    for (size_t i = 0; i < 16; i++) {
        this->state_[i] = v[i];
    }
}

void encrypt::KeystreamGenerator::iv(std::vector<uint8_t>& iv)
{
    this->iv_ = iv;
}

encrypt::KeystreamGenerator::KeystreamGeneratorMode encrypt::KeystreamGenerator::mode() const
{
    return this->mode_;
}

void encrypt::KeystreamGenerator::quarter_round(uint32_t& a, uint32_t& b, uint32_t& c, uint32_t& d){
    a += b; d ^= a; d = rotl32(d, 16);
    c += d; b ^= c; b = rotl32(b, 12);
    a += b; d ^= a; d = rotl32(d, 8);
    c += d; b ^= c; b = rotl32(b, 7);
}

// Циклический сдвиг влево для 32-битного слова
uint32_t encrypt::KeystreamGenerator::rotl32(uint32_t x, int shift) {
    return (x << shift) | (x >> (32 - shift));
}

void encrypt::KeystreamGenerator::run()
{
    std::vector<uint8_t>* gamma = new std::vector<uint8_t>(MAX_CLUSTER_SIZE);
    char toWrite[MAX_CLUSTER_SIZE]{};
    switch (mode_)
    {
    case encrypt::KeystreamGenerator::KeystreamGeneratorMode::GEN:
        try
        {   
            const size_t left = size_ % MAX_CLUSTER_SIZE;
            if (size_ >= MAX_CLUSTER_SIZE) {
                for (size_t i = 0; i < size_; i += MAX_CLUSTER_SIZE)
                {
                    // Генерируем байты гаммы
                    this->generateBytes(gamma->data(), gamma->size());
                    std::memcpy(toWrite, reinterpret_cast<const char*>(gamma->data()), gamma->size());
                    this->_of->write(toWrite, gamma->size());
                } 
            }
            // Дописать остаток
            if (left > 0) {
                this->generateBytes(gamma->data(), left);
                std::memcpy(toWrite, reinterpret_cast<const char*>(gamma->data()), left);
                this->_of->write(toWrite, left);
            }
        }
        catch (const std::wstring& e) {
            this->_ef->write(e);
            mode_ = MODE::README;
            this->readme();
        }
        break;
    case MODE::README:
        this->readme();
        break;
    default:
        this->_ef->write(T::msg({L"KeystreamGenerator",(size_t)this->mode_}));
        this->readme();
        break;
    }
}

void encrypt::KeystreamGenerator::readme()
{
    this->_ef->write(T::msg({L"KeystreamGenerator",(size_t)MODE::README}));
}

std::vector<uint8_t> encrypt::KeystreamGenerator::reedKeyFile(const char* path)
{
    std::vector<uint8_t> out(KEY_SIZE);
    std::ifstream fin(path, std::ios::binary);

    if (!fin.is_open()) {
        throw MODE::NOACCESKF;
    }

    fin.read(reinterpret_cast<char*>(out.data()), KEY_SIZE);

    if (fin.gcount() != KEY_SIZE) {
        throw MODE::NOFULLKF;
    }
    return out;
}

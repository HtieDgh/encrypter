#pragma once
#include <cstdint>
#include <array>
#include <cstring>
#include <stdexcept>
#include <vector>
#include <map>
#include <string>
#include "Algorithm.h"
namespace encrypt {
    /**
     * Генератор ключевой гаммы на основе ChaCha20 с 64-битным счётчиком.
     * Позволяет получить до 2^70 байт (далёкий предел) уникальной псевдослучайной
     * последовательности для одного ключа и IV. Максимальная запрашиваемая длина
     * в условии – 512 ГиБ, что полностью поддерживается.
     *
     * Ключ: 32 байта (256 бит)
     * IV (nonce): 8 байт (64 бита)
     *
     * Состояние ChaCha20 организовано следующим образом (16 слов по 32 бита):
     *   0-3 : константы "expand 32-byte k"
     *   4-11: ключ (8 слов)
     *   12  : счётчик (младшие 32 бита)
     *   13  : счётчик (старшие 32 бита)
     *   14  : nonce (младшие 32 бита)
     *   15  : nonce (старшие 32 бита)
     */
	class KeystreamGenerator : public Algorithm
	{
	public:
        /** 
        * Режимы работы chacha20 геренатора
        */
        enum class KeystreamGeneratorMode
        {
            GEN=99,
            NOOUTPUTGEN=6,
            NOACCESKF=5,
            NOFULLKF=4,
            NOKEYFILE=2,
            NOMODE=3,
            README=1
        };

		static constexpr size_t BLOCK_SIZE = 64;   // байт в одном блоке
        static constexpr size_t DEFAULT_GEN_SIZE = 64;   // сколько будет сгенерировано байт в последовательности
        static constexpr size_t KEY_SIZE = 32; //размер ключа в байтах
        static constexpr size_t MAX_CLUSTER_SIZE = 4096; //размер буффера гаммы, перед тем как запустить снова generateBytes() 

        /**
        * Конструктор.
        * @param key 32-байтовый ключ (256 бит)
        * @param iv  8-байтовый инициализирующий вектор (nonce)
        * @throws std::invalid_argument если размеры не совпадают
        */
        KeystreamGenerator(std::wstring modename, std::map<std::wstring, std::wstring>& params, OutputStrategy* const errout);
        /**
        * Сброс генератора в начальное состояние (счётчик = 0,
        * готовность к выдаче новой последовательности).
        */
        void reset();
        /**
        * Заполняет буфер гаммой заданной длины.
        * @param buffer указатель на область памяти, куда будут записаны байты
        * @param length количество запрашиваемых байт (доступный объём должен быть не менее length)
        */
        void generateBytes(uint8_t* buffer, size_t length);
        // Функция четвертного раунда ChaCha20
        static void quarter_round(uint32_t& a, uint32_t& b, uint32_t& c, uint32_t& d);
        // Циклический сдвиг влево для 32-битного слова
        static uint32_t rotl32(uint32_t x, int shift);

        void run() override;
        void readme() override;
        void mode(KeystreamGeneratorMode v);
        void state(std::array<uint32_t,16> v);
        void iv(std::vector<uint8_t>& iv);
        KeystreamGeneratorMode mode() const;

    private:
        // Внутреннее состояние (до применения quarter rounds)
        std::array<uint32_t,16> state_;
        // Текущий сгенерированный блок (64 байта)
        std::array<uint8_t, BLOCK_SIZE> current_block_;
        // Текущая позиция в блоке (0..BLOCK_SIZE)
        size_t pos_ = BLOCK_SIZE;
        // 64-битный счётчик блоков
        uint64_t counter_ = 0;

        // Сохранённые ключ и IV для сброса
        std::vector<uint8_t> key_;
        std::vector<uint8_t> iv_;

        // Вспомогательная функция: записывает счётчик и IV в состояние
        void set_counter_and_iv(uint64_t counter, const std::vector<uint8_t>& iv);

        // Генерация одного блока (ChaCha20 block function)
        void generate_next_block();
        KeystreamGeneratorMode mode_;
        
        //количество байт для генерации
        unsigned long long size_;

        std::vector<uint8_t> reedKeyFile(const char* path);
	};
}
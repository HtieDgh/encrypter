#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace fs = std::filesystem;

namespace UnitTest
{
	//Базовый абстрактный класс для 
	class MockInputStrategy : public encrypt::InputStrategy {
	public:
		MockInputStrategy(const char* plain, size_t maxpos)
			: _plain(plain), _maxpos(maxpos), _pos(0){};
		long long read(char* data, std::streamsize size = 1) override {
			std::streamsize i = 0;
			for (; i < size; ++i) {
				if (_pos > _maxpos) {
					break;
				}
				data[i] = _plain[_pos];
				++_pos;
			}
			return i;
		};
		long long readln(std::string& data, char delim = '\n') override {
			return 0;
		};
		long long readln(std::wstring& data, wchar_t delim = '\n') override {
			return 0;
		};
		long long read(wchar_t* data, std::streamsize size = 1) override {
			return 0;
		};
		std::string getTrueEncoded() {
			return _encoded;
		}
		std::string getTrueDecoded() {
			return _decoded;
		}
	protected:
		std::string _encoded;
		std::string _decoded;
		const char* _plain;
		size_t _pos, _maxpos;
	};

	//Псевдо input для Ceasar algorithm
	class MockCeasarInputStrategy : public MockInputStrategy
	{
	public:
		MockCeasarInputStrategy(const char* plain, size_t maxpos, const char* key)
			: MockInputStrategy(plain, maxpos)
		{
			size_t kd = 0;
			for (size_t i = 0; i <= maxpos; i++)
			{
				if (kd == strlen(key)) {
					kd = 0;
				}
				this->_encoded += _plain[i] + key[kd];//Верная зашифровка
				this->_decoded += _plain[i] - key[kd];//Верная расшифровка
				++kd;
			}
		};
	};
	//Псевдо input для XTEA 
	class MockXTEAInputStrategy : public MockInputStrategy
	{
	public:
		MockXTEAInputStrategy(const char* plain, size_t maxpos, char* encoded, size_t encSize, char* decoded, size_t decSize)
			: MockInputStrategy(plain, maxpos)
		{
			this->_decoded.assign(decoded, decSize);
			this->_encoded.assign(encoded, encSize);
		}
	};
	class MockOutputStrategy : public encrypt::OutputStrategy
	{
	public:
		MockOutputStrategy() {};
		void write(char* data, std::streamsize size = 1) override {
			res.write(data, size);
		};
		std::stringstream res;
		void write(wchar_t* data, std::streamsize size = 1) override {
			return;
		}
		void write(std::wstring data) {
			return;
		}
	};

	TEST_CLASS(CeasarTest)
	{
	public:
		TEST_METHOD(TestforTest)
		{
			Assert::AreEqual(2, 2);
		}
		TEST_METHOD(InitializedAlgorithmHasReadmeStateTest)
		{
			encrypt::Ceasar a;
			auto stubmode = (uint8_t)encrypt::Ceasar::CeasarMode::README;
			Assert::AreEqual<uint8_t>(stubmode, (uint8_t)a.mode());
		}
		TEST_METHOD(InitializedAlgorithmHasZeroMaxsizeTest) {
			encrypt::Ceasar a;
			Assert::AreEqual<size_t>(0, a.maxsize());
		}

		TEST_METHOD(EncodeTest) {
			auto key = "\x02\x04";
			MockCeasarInputStrategy* MIS = new MockCeasarInputStrategy("1234\x32\x1B\x96\x54\x48", 10, key);
			MockOutputStrategy* MOS = new MockOutputStrategy();
			std::map<std::string, const char*> params{
				{"-k",key}
			};
			encrypt::StderrOutput* errout = new encrypt::StderrOutput();

			encrypt::Ceasar a("enc", params, errout);
			a.setInput(MIS);
			a.setOutput(MOS);
			a.run();

			Assert::AreEqual<std::string>(MIS->getTrueEncoded(), MOS->res.str(), L"Ceasar Трансформация plain->enc не верна");
		}

		TEST_METHOD(DecodeTest) {
			auto key = "\x02\x04";
			MockCeasarInputStrategy* MIS = new MockCeasarInputStrategy("1234\x32\x1B\x96\x54\x48", 10, key);
			MockOutputStrategy* MOS = new MockOutputStrategy();
			std::map<std::string, const char*> params{
				{"-k",key}
			};
			encrypt::StderrOutput* errout = new encrypt::StderrOutput();

			encrypt::Ceasar a("dec", params, errout);
			a.setInput(MIS);
			a.setOutput(MOS);
			a.run();

			Assert::AreEqual<std::string>(MIS->getTrueDecoded(), MOS->res.str(), L"Ceasar Трансформация plain->enc не верна");
		}
	};
	TEST_CLASS(XTEATest)
	{
	public:		
		TEST_METHOD(InitializedAlgorithmHasReadmeStateTest)
		{
			encrypt::XTEA a;
			auto stubmode = (uint8_t)encrypt::XTEA::XTEAMode::README;
			Assert::AreEqual<uint8_t>(stubmode, (uint8_t)a.mode());
		}
		TEST_METHOD(EncodeTest) {
			//Раунды Фестеля 32
			auto key = new uint32_t[4]{
				0x1f55d069,
				0x32a45e6a,
				0x8db6e71b,
				0xdf69d2f7
			};

			// "Привет мир!" CP1251
			char plain[11] =
			{
				0xcf, 0xf0, 0xe8, 0xe2, 0xe5, 0xf2, 0x20, 0xec,
				0xe8, 0xf0, 0x21
			};
			
			char encoded[16] =
			{
				0x59, 0x65, 0x70, 0xb6, 0x02, 0xf9, 0x93, 0x39,
				0x75, 0xe6, 0x67, 0x91, 0xd4, 0xfa, 0x05, 0x9e
			};

			char decoded[16] =
			{
				0xcf, 0xf0, 0xe8, 0xe2, 0xe5, 0xf2, 0x20, 0xec,
				0xe8, 0xf0, 0x21, 0xcb, 0xc0, 0x18, 0x96, 0xa8
			};

			MockXTEAInputStrategy* MIS = new MockXTEAInputStrategy(plain, 10, encoded,16, decoded,16);
			MockOutputStrategy* MOS = new MockOutputStrategy();
			std::map<std::string, const char*> params{};
			encrypt::StderrOutput* errout = new encrypt::StderrOutput();

			encrypt::XTEA a("enc", params, errout);
			a.setInput(MIS);
			a.setOutput(MOS);
			a.setKey(key);
			a.mode(encrypt::XTEA::XTEAMode::ENC);
			a.run();
			std::string s = MOS->res.str();
			Assert::AreEqual<std::string>(MIS->getTrueEncoded(), MOS->res.str(), L"XTEA Трансформация plain->enc не верна");
		}
		TEST_METHOD(DecodeTest) {
			//Раунды Фестеля 32
			auto key = new uint32_t[4]{
				0x1f55d069,
				0x32a45e6a,
				0x8db6e71b,
				0xdf69d2f7
			};

			// "Привет мир!" CP1251 после encode (как-будто это plain)
			char plain[16] =
			{
				0x59, 0x65, 0x70, 0xb6, 0x02, 0xf9, 0x93, 0x39,
				0x75, 0xe6, 0x67, 0x91, 0xd4, 0xfa, 0x05, 0x9e
			};
			char* encoded = nullptr;
			char decoded[16] =
			{
				0xcf, 0xf0, 0xe8, 0xe2, 0xe5, 0xf2, 0x20, 0xec,
				0xe8, 0xf0, 0x21, 0xb6, 0x02, 0xf9, 0x93, 0x39
			};


			MockXTEAInputStrategy* MIS = new MockXTEAInputStrategy(plain, 16, encoded, 0, decoded, 16);
			MockOutputStrategy* MOS = new MockOutputStrategy();
			std::map<std::string, const char*> params{};
			encrypt::StderrOutput* errout = new encrypt::StderrOutput();

			encrypt::XTEA a("dec", params, errout);
			a.setInput(MIS);
			a.setOutput(MOS);
			a.setKey(key);
			a.mode(encrypt::XTEA::XTEAMode::DEC);
			a.run();
			std::string s = MOS->res.str();
			Assert::AreEqual<std::string>(MIS->getTrueDecoded(), MOS->res.str(), L"XTEA Трансформация enc->plain не верна");
		}
	};
}

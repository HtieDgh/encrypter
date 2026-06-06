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
		std::string rawbytesfromwchar(const wchar_t* key) {
			auto s = wcslen(key) * sizeof(wchar_t);
			auto pkey = new char[s + 1] {};
			memcpy(pkey, key, s);
			std::string strkey;
			strkey.resize(s);
			for (size_t i = 0; i < s; i++)
			{
				strkey[i] = pkey[i];
			}
			return strkey;
		}
		void reset() override {
			_pos = 0;
		}
		long long operator>> (std::wstring& data)override {
			return 0;
		}
		long long operator>> (std::string& data)override {
			return 0;
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
		MockCeasarInputStrategy(const char* plain, size_t maxpos, const wchar_t* key)
			: MockInputStrategy(plain, maxpos)
		{
			auto rk = rawbytesfromwchar(key);
			size_t kd = 0;
			for (size_t i = 0; i <= maxpos; i++)
			{
				if (kd == rk.size()) {
					kd = 0;
				}
				this->_encoded += _plain[i] + rk[kd];//Верная зашифровка
				this->_decoded += _plain[i] - rk[kd];//Верная расшифровка
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
		TEST_METHOD(EncodeFromKFTest)
		{
			auto key = L"\x02\x04";

			MockInputStrategy* MKF = new MockInputStrategy("\x48\x1B\x37\xa2\x48""4321", 8);
			MockInputStrategy* MIS = new MockInputStrategy("1234\x32\x1B\x96\x54\x48""serd", 12);
			auto trueEncoded = std::string{
				(char)('1' + '\x48'),
				(char)('2' + '\x1B'),
				(char)('3' + '\x37'),
				(char)('4' + '\xa2'),
				(char)('\x32' + '\x48'),
				(char)('\x1B' + '4'),
				(char)('\x96' + '3'),
				(char)('\x54' + '2'),
				(char)('\x48'+'1'),
				(char)('s'+'\x48'),
				(char)('e' + '\x1B'),
				(char)('r' + '\x37'),
				(char)('d' + '\xa2')
			};
			MockOutputStrategy* MOS = new MockOutputStrategy();
			std::map<std::wstring, std::wstring> params{
				{L"-k",key}
			};
			encrypt::StderrOutput* errout = new encrypt::StderrOutput();

			encrypt::Ceasar a(L"enc", params, errout);
			a.setInput(MIS);
			a.setOutput(MOS);
			a.setKeyFile(MKF);
			a.run();

			Assert::AreEqual<std::string>(trueEncoded, MOS->res.str(), L"Ceasar Трансформация plain->enc не верна");
		}
		TEST_METHOD(DecodeFromKFTest)
		{
			auto key = L"\x02\x04";

			MockInputStrategy* MKF = new MockInputStrategy("\x48\x1B\x37\xa2\x48""4321", 8);
			MockInputStrategy* MIS = new MockInputStrategy("1234\x32\x1B\x96\x54\x48""serd", 12);
			auto trueEncoded = std::string{
				(char)('1' - '\x48'),
				(char)('2' - '\x1B'),
				(char)('3' - '\x37'),
				(char)('4' - '\xa2'),
				(char)('\x32' - '\x48'),
				(char)('\x1B' - '4'),
				(char)('\x96' - '3'),
				(char)('\x54' - '2'),
				(char)('\x48' - '1'),
				(char)('s' - '\x48'),
				(char)('e' - '\x1B'),
				(char)('r' - '\x37'),
				(char)('d' - '\xa2')
			};
			MockOutputStrategy* MOS = new MockOutputStrategy();
			std::map<std::wstring, std::wstring> params{
				{L"-k",key}
			};
			encrypt::StderrOutput* errout = new encrypt::StderrOutput();

			encrypt::Ceasar a(L"dec", params, errout);
			a.setInput(MIS);
			a.setOutput(MOS);
			a.setKeyFile(MKF);
			a.run();

			Assert::AreEqual<std::string>(trueEncoded, MOS->res.str(), L"Ceasar Трансформация plain->enc не верна");
		}
		TEST_METHOD(EncodeTest) {
			auto key = L"\x02\x04";


			MockCeasarInputStrategy* MIS = new MockCeasarInputStrategy("1234\x32\x1B\x96\x54\x48", 10, key);
			MockOutputStrategy* MOS = new MockOutputStrategy();
			std::map<std::wstring, std::wstring> params{
				{L"-k",key}
			};
			encrypt::StderrOutput* errout = new encrypt::StderrOutput();

			encrypt::Ceasar a(L"enc", params, errout);
			a.setInput(MIS);
			a.setOutput(MOS);
			a.run();

			Assert::AreEqual<std::string>(MIS->getTrueEncoded(), MOS->res.str(), L"Ceasar Трансформация plain->enc не верна");
		}

		TEST_METHOD(DecodeTest) {
			auto key = L"\x02\x04";
			MockCeasarInputStrategy* MIS = new MockCeasarInputStrategy("1234\x32\x1B\x96\x54\x48", 10, key);
			MockOutputStrategy* MOS = new MockOutputStrategy();
			std::map<std::wstring, std::wstring> params{
				{L"-k",key}
			};
			encrypt::StderrOutput* errout = new encrypt::StderrOutput();

			encrypt::Ceasar a(L"dec", params, errout);
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
			std::map<std::wstring, std::wstring> params{};
			encrypt::StderrOutput* errout = new encrypt::StderrOutput();

			encrypt::XTEA a(L"enc", params, errout);
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
			std::map<std::wstring, std::wstring> params{};
			encrypt::StderrOutput* errout = new encrypt::StderrOutput();

			encrypt::XTEA a(L"dec", params, errout);
			a.setInput(MIS);
			a.setOutput(MOS);
			a.setKey(key);
			a.mode(encrypt::XTEA::XTEAMode::DEC);
			a.run();
			std::string s = MOS->res.str();
			Assert::AreEqual<std::string>(MIS->getTrueDecoded(), MOS->res.str(), L"XTEA Трансформация enc->plain не верна");
		}
	};
	TEST_CLASS(KeystreamGeneratorTest) {
		TEST_METHOD(gammaEqualTest) {
			// Инициализация состояния константами
			const uint32_t constants[4] = {
				0x61707865,  // "expa"
				0x3320646e,  // "nd 3"
				0x79622d32,  // "2-by"
				0x6b206574   // "te k"
			};

			std::vector<uint8_t> key = {0xa6, 0x2d, 0x49, 0xe3, 0x19, 0x88, 0xb3, 0x36, 0x6d, 0xb9, 0x9a, 0x6d, 0x95, 0xd3, 0x11, 0x52};
			std::vector<uint8_t> iv{0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07};

			std::array<uint32_t,16> mockState{};

			//константы
			std::memcpy(mockState.data(), constants, 16);

			// Ключ (слова 4..11)
			std::memcpy(mockState.data() + 4, key.data(), 32);

			mockState[12] = 0;
			mockState[13] = 0;
			mockState[14] = 0;
			mockState[15] = 0;

			unsigned long long size = 50;
			uint8_t* gamma1 = new uint8_t[size];
			uint8_t* gamma2 = new uint8_t[size];

			std::map<std::wstring, std::wstring> params{};
			encrypt::StderrOutput* errout = new encrypt::StderrOutput();
			encrypt::KeystreamGenerator a(L"gen", params, errout);
			a.mode(encrypt::KeystreamGenerator::KeystreamGeneratorMode::GEN);
			a.state(mockState);
			a.iv(iv);
			a.reset();

			a.generateBytes(gamma1, size);

			a.reset();

			a.generateBytes(gamma2, size);

			for (size_t i = 0; i < size; i++)
			{
				Assert::AreEqual<uint8_t>(gamma1[i], gamma2[i], L"Gamma1 и Gamma2 не равны");
			}

		}
		TEST_METHOD(gammaFileGenerationEqual) {
			std::map<std::wstring, std::wstring> params{};
			params[L"-kf"] = LR"(C:\git_repos\encrypter\test\UnitTest\chacha20.key)";
			params[L"-size"] = LR"(50)";
			params[L"-of"] = LR"(C:\git_repos\encrypter\test\UnitTest\gamma.key)";
			encrypt::StderrOutput* errout = new encrypt::StderrOutput();
			auto MOS1 = new MockOutputStrategy();
			auto MOS2 = new MockOutputStrategy();

			auto a = new encrypt::KeystreamGenerator(L"gen", params, errout);
			a->setOutput(MOS1);
			a->run();
			auto b = new encrypt::KeystreamGenerator(L"gen", params, errout);
			b->setOutput(MOS2);
			b->run();
			Assert::AreEqual<std::string>(MOS1->res.str(), MOS2->res.str(), L"No way!!! гаммы в файлах не равны");
		};
	};

}

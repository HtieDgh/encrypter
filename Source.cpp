#include<string>
#include<iostream>
#include<iomanip>
#include<locale.h>
#include<Windows.h>
#include<map>
#include "Ceasar.h"
#include "AlgorithmStrategy.h"

using namespace std;
using namespace encrypt;
#define README_PATH ""

// ��������� ���������� ������

// ����� �������
void readme(std::ostream& out) {
	out << "�������������: encrypter {��� ���������} {dec|enc} [�����] < \\path\\to\\file.txt > \\path\\to\\encrypted.txt\n";
}
void Creadme(std::ostream& out) {
	out << "������������� ceasar:\n"
		<<"\tencrypter ceasar enc - k \"key to encrypt\" -size 10 -of \\path\\to\\encrypted.txt\n < \\path\\to\\file.txt"
		<<"���\n"
		<<"\tencrypter ceasar dec -k \"key to decrypt\" -size 10 -of \\path\\to\\decrypted.txt < path\\to\\encrypted.txt\n";
}




int main(int argc, char* argv[], char* envp[]){
	setlocale(LC_ALL, ".1251");
	SetConsoleCP(1251);
	size_t inSize = 100;
	AlgorithmStrategy* controller=new AlgorithmStrategy();
	map<string,const char*> params;
	//����������� ���������� � ��������
	for (int i = 3; i < argc; i++) {
		if (argv[i][0] == '-' && i + 1 < argc && argv[i + 1][0] != '-') {
		// �������� �� ���������
			params[argv[i]] = argv[i + 1];
		}
		else if (argv[i][0] == '-') {
		// �������� ��� ��������, �������� ����
			params[argv[i]] = "";
		}
	}
	if (argc <= 1) {
		readme(cout);
		return 0;
	} 

	if (!strcmp(argv[1], "ceasar")) {
		Ceasar* alg = new Ceasar();
		controller->setAlgorithm(alg);

		if (!strcmp(argv[2], "enc")) {
			alg->mode(false);
		}
		else if (!strcmp(argv[2], "dec")) {
			alg->mode(true);
		}else {
			Creadme(cout);
			return 0;
		}

	// ��������� ���������� ��������� ������ � ���������� � ����������
		try
		{
			if (params.count("-k") == 0)
			{
				throw std::logic_error("�������� -k �� �����");
			}

			alg->key(params["-k"]);

			if (params.count("-size") == 0)
			{
				throw std::logic_error("�������� -size �� �����");
			}

			alg->maxsize(atoi(params["-size"])); //atoi() ��������� char* � int

			if (params.count("-of") == 0)
			{
				// �� ��������� ����� � stdout
				alg->setOutput(new StdoutOutput());
			} else {
				// ���� �� ����� ��������, ����� � ���� ����
				alg->setOutput(new FileOutput(params["-of"]));
			}

			// ���������� �������
			alg->run();
			return 0;
		}
		catch (const std::exception& e)
		{
			cerr << e.what() << endl;
			Creadme(cout);
		}
	}
}
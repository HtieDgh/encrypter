#include<string>
#include<iostream>
#include<iomanip>
#include<locale.h>
#include<Windows.h>
#include "Ceaser.h"
#include "SetAlgoritm.h"

using namespace std;
using namespace encrypt;
#define README_PATH ""
// ��������� ���������� ������

// ����� �������
void readme() {
	cout << "�������������: encrypter {��� ���������} {dec|enc} [�����] < \\path\\to\\file.txt > \\path\\to\\encrypted.txt\n";
}
void Creadme() {
	cout << "������������� ceasar:\n"
		<<"\tencrypter ceasar enc - k \"key to encrypt\" -size 10 < \\path\\to\\file.txt > \\path\\to\\encrypted.txt\n"
		<<"���\n"
		<<"\tencrypter ceasar dec -k \"key to decrypt\" -size 10 < path\\to\\encrypted.txt > \\path\\to\\decrypted.txt\n";
}




int main(int argc, char* argv[], char* envp[])
{
	setlocale(LC_ALL, ".1251");
	SetConsoleCP(1251);
	size_t inSize = 100;
	SetAlgoritm* controller=new SetAlgoritm();
	if (argc > 1) {
		if (!strcmp(argv[1], "ceasar")) {
			Ceaser* alg = new Ceaser();
			controller->setAlgoritm(alg);

			if (!strcmp(argv[2], "enc")) {
				alg->mode(false);
			}
			else if (!strcmp(argv[2], "dec")) {
				alg->mode(true);
			}else {
				Creadme();
				return 0;
			}
			for (int i = 3; i < argc; i++) {
				if (!strcmp(argv[i], "-k")) {
					alg->key(argv[i + 1]);
				}
				if (!strcmp(argv[i], "-size")) {
					alg->maxsize(atoi(argv[i + 1])); //atoi() ��������� char* � int
				}
			}

			if (alg->key() != "default" || alg->maxsize() != -1) {
				alg->run();
				return 0;
			}
			Creadme();
			return 0;
		}
	}
	readme();
	return 0;

}
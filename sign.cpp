#include "sign.h"
#include <fstream>
#include <iostream>
#include <string>
#include "BankSign.h"

int main(int argc, char *argv[])
{
	std::ifstream ifs(argv[1]);
	char oldSignature[100] = "";
	char newSignature[100] = "";
	char name[100] = "";
	std::string content( (std::istreambuf_iterator<char>(ifs) ),
		(std::istreambuf_iterator<char>()    ) );

	GetCurrentSig(content.c_str(), oldSignature, sizeof(oldSignature));
	//        author   user     name     data             buf           bufsize
	GetNewSig(argv[2], argv[3], argv[4], content.c_str(), newSignature, sizeof(newSignature));

	std::cout << "Old signature: " << oldSignature << std::endl;
	std::cout << "New signature: " << newSignature << std::endl;

	return 0;
}

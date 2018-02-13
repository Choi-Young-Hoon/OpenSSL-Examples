#include <iostream>
#include <string>

#include "CRYPTO_Example/Sha.h"

int main(void) {
	SHA sha;
	char data[] = "Hello World";

	for (int i = 0; i < 3; i++) {
		sha.SetData((unsigned char*)data, strlen(data));
		sha.Sha256();
		std::cout << "SHA256 :  " << sha.GetData() << std::endl;

		sha.SetData((unsigned char*)data, strlen(data));
		sha.Sha512();
		std::cout << "SHA512 :  " << sha.GetData() << std::endl << std::endl << std::endl;
	}
	return 0;
}
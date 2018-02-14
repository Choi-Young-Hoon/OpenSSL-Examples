#include <iostream>
#include <string>

#include "CRYPTO_Example/AES.h"
#include "CRYPTO_Example/Base64.h"

int main(void) {
	AES aes;
	Base64 base64;

	std::string strData = "Hello World";

	base64.SetData(strData.c_str());
	base64.Encode();
	std::cout << "Plain Data: " << base64.GetData() << std::endl;

	///////////////////////////////////////////////////// 
	////				  AES 128 bit			  	  ///
	/////////////////////////////////////////////////////
	std::cout << "AES 128 Test ===============================================" << std::endl;
 	aes.SetData(strData.c_str());
	aes.CreateKey128();
	aes.Encrypt();

	base64.SetData(aes.GetData(), aes.GetDataLength());
	base64.Encode();
	std::cout << "Encrypt Data: " << base64.GetData() << std::endl;
	
	aes.Decrypt();
	
	base64.SetData(aes.GetData(), aes.GetDataLength());
	base64.Encode();
	std::cout << "Decrypt Data: " << base64.GetData() << std::endl;
	



	///////////////////////////////////////////////////// 
	////				  AES 192 bit			  	  ///
	/////////////////////////////////////////////////////
	std::cout << "AES 192 Test ===============================================" << std::endl;
	aes.SetData(strData.c_str());
	aes.CreateKey192();
	aes.Encrypt();

	base64.SetData(aes.GetData(), aes.GetDataLength());
	base64.Encode();
	std::cout << "Encrypt Data: " << base64.GetData() << std::endl;

	aes.Decrypt();

	base64.SetData(aes.GetData(), aes.GetDataLength());
	base64.Encode();
	std::cout << "Decrypt Data: " << base64.GetData() << std::endl;


	///////////////////////////////////////////////////// 
	////				  AES 256 bit			  	  ///
	/////////////////////////////////////////////////////
	std::cout << "AES 256 Test ===============================================" << std::endl;
	aes.SetData(strData.c_str());
	aes.CreateKey256();
	aes.Encrypt();

	base64.SetData(aes.GetData(), aes.GetDataLength());
	base64.Encode();
	std::cout << "Encrypt Data: " << base64.GetData() << std::endl;

	aes.Decrypt();

	base64.SetData(aes.GetData(), aes.GetDataLength());
	base64.Encode();
	std::cout << "Decrypt Data: " << base64.GetData() << std::endl;

	return 0;
}

/*
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
*/
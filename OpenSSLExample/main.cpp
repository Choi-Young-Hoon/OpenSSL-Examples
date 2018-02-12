#include <iostream>
#include <string>

#include "Base64.h"

int main(void) {
	Base64 base64;
	char szData[] = "Hello World";
	char* szBase64Data = NULL;

	base64.Base64Encode((unsigned char*)szData, strlen(szData), &szBase64Data);
	std::cout << "Plain Data: " << szData << std::endl;
	std::cout << "Plain Data Length: " << strlen(szData) << std::endl;
	std::cout << "Base64 Encode Data: " << szBase64Data << std::endl;

	char* pszDecodeData = NULL;
	unsigned int nDecodeDataLength = 0;
	base64.Base64Decode(szBase64Data, (unsigned char**)&pszDecodeData, &nDecodeDataLength);
	std::cout << "Base64 Decode Data: " << pszDecodeData << std::endl;
	std::cout << "Bsee64 Decode Data Length: " << nDecodeDataLength << std::endl;

	return 0;
}
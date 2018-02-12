#ifndef __BASE64_HEADER__
#define __BASE64_HEADER__

#include "openssl/bio.h"
#include "openssl/evp.h"
#include "openssl/buffer.h"

class Base64 {
public:
	explicit Base64();
	virtual ~Base64();
	
	int Base64Encode(const unsigned char* pData, unsigned int nDataLength, char** pEncodeData);
	int	Base64Decode(char* pBase64Data, unsigned char** pDecodeData, unsigned int* pDecodeDataLength);

protected:
	int CalcDecodeLength(const char* pBase64Data);

private:
	
};

#endif
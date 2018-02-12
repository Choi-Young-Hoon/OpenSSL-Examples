#include "Base64.h"

#include <cstring>

Base64::Base64()
{}

Base64::~Base64()
{}



int Base64::Base64Encode(const unsigned char* pData, unsigned int nDataLength, char** pEncodeData)
{
	BIO* pBio, 
	   * pBase64;
	BUF_MEM* pBufferPtr;

	pBase64 = BIO_new(BIO_f_base64());
	pBio	 = BIO_new(BIO_s_mem());
	pBio	 = BIO_push(pBase64, pBio);

	BIO_set_flags(pBio, BIO_FLAGS_BASE64_NO_NL);
	BIO_write(pBio, pData, nDataLength);
	BIO_flush(pBio);

	BIO_get_mem_ptr(pBio, &pBufferPtr);
	BIO_set_close(pBio, BIO_NOCLOSE);
	BIO_free_all(pBio);

	*pEncodeData = (*pBufferPtr).data;

	return 0;
}



int Base64::Base64Decode(char* pBase64Data, unsigned char** pDecodeData, unsigned int* pDecodeDataLength) {
	BIO* pBio,
	   * pBase64;

	int nDecodeLength = CalcDecodeLength(pBase64Data);
	*pDecodeData = new unsigned char[nDecodeLength];
	(*pDecodeData)[nDecodeLength] = '\0';

	pBio = BIO_new_mem_buf(pBase64Data, -1);
	pBase64 = BIO_new(BIO_f_base64());
	pBio = BIO_push(pBase64, pBio);

	BIO_set_flags(pBio, BIO_FLAGS_BASE64_NO_NL);
	*pDecodeDataLength = BIO_read(pBio, *pDecodeData, strlen(pBase64Data));
	//assert(*pDecodeDataLength == nDecodeLength);
	BIO_free_all(pBio);

	return 0;
}



int Base64::CalcDecodeLength(const char* pBase64Data) {
	int nDataLength = strlen(pBase64Data),
		nPadding = 0;

	if (pBase64Data[nDataLength - 1] == '=' &&
		pBase64Data[nDataLength - 2] == '=') {
		nPadding = 2;
	} else if (pBase64Data[nDataLength - 1] == '=') {
		nPadding = 1;
	}

	return (nDataLength * 3) / 4 - nPadding;
}
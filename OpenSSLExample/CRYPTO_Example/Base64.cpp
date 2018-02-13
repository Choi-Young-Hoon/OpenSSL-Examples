#include "Base64.h"

#include <cstring>

Base64::Base64()
: m_pData(NULL)
, m_nDataLength(0)
{}

Base64::~Base64() {
	Clear();
}



bool Base64::Encode() {
	if (m_pData == NULL) {
		return false;
	}

	BIO* pBio, 
	   * pBase64;
	BUF_MEM* pBufferPtr;

	pBase64	= BIO_new(BIO_f_base64());
	pBio	= BIO_new(BIO_s_mem());
	pBio	= BIO_push(pBase64, pBio);

	BIO_set_flags(pBio, BIO_FLAGS_BASE64_NO_NL);
	BIO_write(pBio, m_pData, m_nDataLength);
	BIO_flush(pBio);

	BIO_get_mem_ptr(pBio, &pBufferPtr);

	BIO_set_close(pBio, BIO_NOCLOSE);
	BIO_free_all(pBio);

	if (!SetData((unsigned char*)pBufferPtr->data, pBufferPtr->length)) {
		return false;
	}

	return true;
}



bool Base64::Decode() {
	if (m_pData == NULL) {
		return false;
	}

	BIO* pBio,
	   * pBase64;

	int nDecodeLength = CalcDecodeLength((char*)m_pData, m_nDataLength);
	unsigned char* pDecodeData = new unsigned char[nDecodeLength];
	if (pDecodeData == NULL) {
		return false;
	}
	memset(pDecodeData, 0x00, nDecodeLength);

	pBio	 = BIO_new_mem_buf(m_pData, m_nDataLength);
	pBase64 = BIO_new(BIO_f_base64());
	pBio	 = BIO_push(pBase64, pBio);

	BIO_set_flags(pBio, BIO_FLAGS_BASE64_NO_NL);
	nDecodeLength = BIO_read(pBio, pDecodeData, m_nDataLength);
	BIO_free_all(pBio);
	
	if (!SetData(pDecodeData, nDecodeLength)) {
		delete[] pDecodeData;
		return false;
	}

	delete[] pDecodeData;

	return true;
}



bool Base64::SetData(const char* pData) {
	return SetData((const unsigned char*)pData, strlen(pData));
}



bool Base64::SetData(const unsigned char* pData, const unsigned int nDataLength) {
	Clear();

	m_pData = new unsigned char[nDataLength];
	if (m_pData == NULL) {
		return false;
	}
	memset(m_pData, 0x00, nDataLength);
	m_nDataLength = nDataLength;

	memcpy(m_pData, pData, nDataLength);

	return true;
}



void Base64::Clear() {
	if (m_pData != NULL) {
		delete[] m_pData;

		m_pData		  = NULL;
		m_nDataLength = 0;
	}
}



int Base64::CalcDecodeLength(const char* pBase64Data, const unsigned int nBase64DataLength) {
	int nPadding = 0;

	if (pBase64Data[nBase64DataLength - 1] == '=' &&
		pBase64Data[nBase64DataLength - 2] == '=') {
		nPadding = 2;
	} else if (pBase64Data[nBase64DataLength - 1] == '=') {
		nPadding = 1;
	}

	return (nBase64DataLength * 3) / 4 - nPadding;
}



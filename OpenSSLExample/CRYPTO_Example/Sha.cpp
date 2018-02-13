#include "SHA.h"

#include "Base64.h"

SHA::SHA()
: m_pData(NULL)
, m_nDataLength(0)
{}

SHA::~SHA()
{}



bool SHA::Sha256() {
	// 아래처럼 함수 한번에도 가능함.
	// SHA256(m_pData, m_nDataLength, (unsigned char*)&digest);

	unsigned char digest[SHA256_DIGEST_LENGTH] = { 0, };

	SHA256_CTX ctx;
	SHA256_Init(&ctx);
	SHA256_Update(&ctx, m_pData, m_nDataLength);
	SHA256_Final(digest, &ctx);

	if (!SetData(digest, SHA256_DIGEST_LENGTH)) {
		return false;
	}

	return true;
}



bool SHA::Sha512() {
	// 아래처럼 함수 한번에도 가능함.
	//SHA512(m_pData, m_nDataLength, (unsigned char*)&digest);

	unsigned char digest[SHA512_DIGEST_LENGTH] = { 0, };

	SHA512_CTX ctx;
	SHA512_Init(&ctx);
	SHA512_Update(&ctx, m_pData, m_nDataLength);
	SHA512_Final(digest, &ctx);

	if (!SetData(digest, SHA512_DIGEST_LENGTH)) {
		return false;
	}

	return true;
}



bool SHA::SetData(unsigned char* pData, unsigned int nDataLength) {
	Clear();

	m_pData = new unsigned char[nDataLength];
	if (pData == NULL) {
		return false;
	}
	memset(m_pData, 0x00, nDataLength);
	m_nDataLength = nDataLength;

	memcpy(m_pData, pData, m_nDataLength);

	return true;
}



std::string SHA::GetData() {
	Base64 base64;

	base64.SetData(m_pData, m_nDataLength);
	base64.Encode();

	return base64.GetData();
}



void SHA::Clear() {
	if (m_pData != NULL) {
		delete[] m_pData;

		m_pData		  = NULL;
		m_nDataLength = 0;
	}
}
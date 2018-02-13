#ifndef __BASE64_HEADER__
#define __BASE64_HEADER__

#include <string>

#include "openssl/bio.h"
#include "openssl/evp.h"
#include "openssl/buffer.h"

class Base64 {
public:
	explicit Base64();
	virtual ~Base64();
	
	bool Encode();
	bool Decode();

public:
	bool SetData(const char* pData);
	bool SetData(const unsigned char* pData, const unsigned int nDataLength);

	std::string GetData() { return std::string(reinterpret_cast<char*>(m_pData), m_nDataLength); }

	void Clear();

protected:
	int CalcDecodeLength(const char* pBase64Data, const unsigned int nBase64DataLength);

private:
	unsigned char* m_pData;
	unsigned int   m_nDataLength;
};

#endif
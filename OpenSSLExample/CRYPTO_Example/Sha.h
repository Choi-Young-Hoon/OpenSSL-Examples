#ifndef __SHA_HEADER__
#define __SHA_HEADER__

#include <string>

#include "openssl/sha.h"

class SHA
{
public:
	explicit SHA();
	virtual ~SHA();

	// SHA256
	bool Sha256();

	// SHA512
	bool Sha512();

public:
	bool SetData(unsigned char* pData, unsigned int nDataLength);

	std::string GetData();

	void Clear();

private:
	unsigned char* m_pData;
	unsigned int   m_nDataLength;

};
#endif
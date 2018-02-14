#ifndef __AES_HEADER__
#define __AES_HEADER__

#include <cstring>

#include "openssl/evp.h"
#include "openssl/conf.h"
#include "openssl/rand.h"

class AES {
public:
	explicit AES();
	virtual ~AES();


public:
	bool CreateKey128();
	bool CreateKey192();
	bool CreateKey256();
	
	bool Encrypt();
	bool Decrypt();

	bool SetData(const char* pData);
	bool SetData(const unsigned char* pData, const int nDataLength);

	unsigned char* GetData()	   { return m_pData; }
	unsigned int   GetDataLength() { return m_nDataLength; }


protected:
	bool CreateKey(int nKeyLength);
	bool CreateIV();
	
	void Clear();
	void ClearKey();
	void ClearIV();


private:
	unsigned char* m_pData;
	unsigned int   m_nDataLength;


	EVP_CIPHER* m_pAesBitMechanism;

	unsigned char* m_pKey;
	unsigned int   m_nKeyLength;

	unsigned char* m_pIV;
	unsigned int   m_nIVLength;
};
#endif
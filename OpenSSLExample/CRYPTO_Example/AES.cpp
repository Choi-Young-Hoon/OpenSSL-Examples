#include "AES.h"


AES::AES()
: m_pData(NULL), m_pKey(NULL), m_pIV(NULL)
, m_nDataLength(0), m_nKeyLength(0), m_nIVLength(0)
{}

AES::~AES() {
	Clear();
	ClearKey();
	ClearIV();
}



bool AES::CreateKey128() {
	if (!CreateKey(128)) {
		return false;
	}

	if (!CreateIV()) {
		return false;
	}

	m_pAesBitMechanism = (EVP_CIPHER*)EVP_aes_128_cbc();
	return true;
}

bool AES::CreateKey192() {
	if (!CreateKey(192)) {
		return false;
	}

	if (!CreateIV()) {
		return false;
	}

	m_pAesBitMechanism = (EVP_CIPHER*)EVP_aes_192_cbc();
	return true;
}

bool AES::CreateKey256() {
	if (!CreateKey(256)) {
		return false;
	}

	if (!CreateIV()) {
		return false;
	}

	m_pAesBitMechanism = (EVP_CIPHER*)EVP_aes_256_cbc();
	return true;
}



bool AES::CreateKey(int nKeyLength) {
	ClearKey();

	int keyLength = nKeyLength / 8;
	m_pKey = new unsigned char[keyLength];
	if (m_pKey == NULL) {
		return false;
	}
	m_nKeyLength = keyLength;
	memset(m_pKey, 0x00, keyLength);

	if (!RAND_bytes(m_pKey, keyLength)) {
		return false;
	}

	return true;
}



bool AES::CreateIV() {
	ClearIV();

	m_pIV = new unsigned char[EVP_MAX_IV_LENGTH];
	if (m_pIV == NULL) {
		return false;
	}
	m_nIVLength = EVP_MAX_IV_LENGTH;
	memset(m_pIV, 0x00, m_nIVLength);

	if (!RAND_bytes(m_pIV, EVP_MAX_IV_LENGTH)) {
		return false;
	}

	return true;
}



bool AES::SetData(const char* pData) {
	return SetData((unsigned char*)pData, strlen(pData));
}



bool AES::Encrypt() {
	//unsigned long err;
	
	int encryptTotalLength = 0;
	int encryptLength		 = 0;

	unsigned char* encryptData = new unsigned char[m_nDataLength];
	if (encryptData == NULL) {
		return false;
	}
	memset(encryptData, 0x00, m_nDataLength);

	//ERR_load_crypto_strings();

	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);
	if (EVP_EncryptInit(ctx, m_pAesBitMechanism, m_pKey, NULL) != 1) {
		//err = ERR_get_error();
        //printf("ERR: EVP_EncryptInit() - %s\n", ERR_error_string (err, NULL));
        return false;
     }

	if (EVP_EncryptUpdate(ctx, encryptData, &encryptLength, m_pData, m_nDataLength) != 1) {
        //err = ERR_get_error();
        //printf("ERR: EVP_EncryptUpdate() - %s\n", ERR_error_string (err, NULL));
        return false;
    }
	encryptTotalLength = encryptLength;

	if (EVP_EncryptFinal(ctx, &encryptData[encryptTotalLength], &encryptLength) != 1) {
        //err = ERR_get_error();
        //printf("ERR: EVP_EncryptFinal() - %s\n", ERR_error_string (err, NULL));
        return false;
    }
	encryptTotalLength += encryptLength;
    EVP_CIPHER_CTX_cleanup(ctx);
	EVP_CIPHER_CTX_free(ctx);

	//ERR_free_strings();

	SetData(encryptData, encryptTotalLength);

	return true;
}



bool AES::Decrypt() {
	//unsigned long err = 0;
	
	int decryptTotalLength = 0;
	int decryptLength		 = 0;

	unsigned char* decryptData = new unsigned char[m_nDataLength];
	if (decryptData == NULL) {
		return false;
	}
	memset(decryptData, 0x00, m_nDataLength);

	//ERR_load_crypto_strings();

	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	EVP_CIPHER_CTX_init(ctx);
	if (EVP_DecryptInit(ctx, m_pAesBitMechanism, m_pKey, NULL) != 1) {
		//err = ERR_get_error();
		//printf("ERR: EVP_DecryptInit() - %s\n", ERR_error_string(err, NULL));
		return false;
	}

	if (EVP_DecryptUpdate(ctx, decryptData, &decryptLength, m_pData, m_nDataLength) != 1) {
		//err = ERR_get_error();
		//printf("ERR: EVP_DecryptUpdate() - %s\n", ERR_error_string(err, NULL));
		return false;
	}
	decryptTotalLength = decryptLength;

	if (EVP_DecryptFinal(ctx, &decryptData[decryptLength], &decryptLength) != 1) {
		//err = ERR_get_error();
		//printf("ERR: EVP_DecryptFinal() - %s\n", ERR_error_string(err, NULL));
		return false;
	}
	decryptTotalLength += decryptLength;
	EVP_CIPHER_CTX_cleanup(ctx);
	EVP_CIPHER_CTX_free(ctx);

	//ERR_free_strings();

	SetData(decryptData, decryptTotalLength);

	return true;
}



bool AES::SetData(const unsigned char* pData, const int nDataLength) {
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



void AES::Clear() {
	if (m_pData != NULL) {
		delete[] m_pData;

		m_pData		  = NULL;
		m_nDataLength = 0;
	}
}



void AES::ClearKey() {
	if (m_pKey != NULL) {
		delete[] m_pKey;

		m_pKey		 = NULL;
		m_nKeyLength = 0;
	}
}



void AES::ClearIV() {
	if (m_pIV != NULL) {
		delete[] m_pIV;

		m_pIV		= NULL;
		m_nIVLength = 0;
	}
}
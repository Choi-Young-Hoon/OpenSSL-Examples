#ifndef __SSL_CLIENT_HEADER__
#define __SSL_CLIENT_HEADER__

#ifdef _WIN32
#include <Windows.h>
#elif __linux__
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

typedef int SOCKET;
#endif

#include <iostream>
#include <string>

#include "openssl/ssl.h"
#include "openssl/err.h"

#define DEFAULT_SERVER_PORT 10000

class SSL_Client {
public:
	explicit SSL_Client();
	virtual ~SSL_Client();

	void Initialize();
	void Finalize();


	SOCKET CreateTcpSocket();
	SOCKET CreateTcpSocket(int nPort);

	SSL_CTX* CreateSslCtx();

	void Start();

private:
	SOCKET   m_Socket;
	SSL_CTX* m_pSSLCtx;
	SSL*	 m_pSSL;
};

#endif
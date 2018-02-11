#ifndef __SSL_SERVER_HEADER__
#define __SSL_SERVER_HEADER__

#ifdef _WIN32
#include <WinSock2.h>
#include <Windows.h>
#pragma comment(lib, "Ws2_32.lib")
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

#define DEFAULT_PORT 10000
#define DEFAULT_CERT_FILE_PATH "my.crt"
#define DEFAULT_KEY_FILE_PATH  "my.key"

class SSL_Server {
public:
	explicit SSL_Server();
	virtual ~SSL_Server();

	void Initialize();
	void Finalize();


	void ServerStart();
	void ClientEcho();

protected:
	// --  TCP Socket 积己  -- //
	SOCKET CreateTcpSocket();
	SOCKET CreateTcpSocket(int nPort);
	
	// --  SSL_CTX  积己  -- //
	SSL_CTX* CreateSslCtx();
	SSL*     CreateClientCtx(SOCKET clientSock);

	// --  Cert, Key 颇老 汲沥.  -- //
	bool ConfigureContext();
	bool ConfigureContext(std::string strCertPath, std::string strKeyPem);


private:
	SOCKET   m_ServerSocket;
	SSL_CTX* m_pServerSslCtx;
};
#endif

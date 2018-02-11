#include "SSL_Server.h"


SSL_Server::SSL_Server()
: m_pServerSslCtx(NULL)
, m_ServerSocket(0)
{}

SSL_Server::~SSL_Server() {
	if (m_pServerSslCtx == NULL) {
		SSL_CTX_free(m_pServerSslCtx);
	}

	if (m_ServerSocket != 0) {
#ifdef _WIN32
		closesocket(m_ServerSocket);
#elif __linux__
		close(m_ServerSocket);
#endif
	}
}



void SSL_Server::Initialize() {
	SSL_load_error_strings();
	SSL_library_init();
	SSLeay_add_ssl_algorithms();

	// SSL/TLS 암호문, 다이제스트를 초기화한다.
	// 다른 OpenSSL 함수를 호출하기 전에 항상 해당 함수로 초기화 해야한다.  
}



void SSL_Server::Finalize() {
#ifdef _WIN32
	WSACleanup();
#endif
}



void SSL_Server::ServerStart() {
	Initialize();

	m_ServerSocket = CreateTcpSocket();
	if (m_ServerSocket == -1) {
		std::cout << "CreateTcpSocket() failed" << std::endl;
		return;
	}

	m_pServerSslCtx = CreateSslCtx();
	if (m_pServerSslCtx == NULL) {
		std::cout << "CreateSslCtx() failed" << std::endl;
		return;
	}

	if (!ConfigureContext()) {
		std::cout << "ConfigureContext() failed" << std::endl;
		return ;
	}

	std::cout << __FUNCTION__ << " - SSL Echo Server Start ========================= " << std::endl;
	ClientEcho();
	std::cout << __FUNCTION__ << " - SSL Echo Server End =========================== " << std::endl;
	
	Finalize();
}



void SSL_Server::ClientEcho() {
	struct sockaddr_in clientAddr;
	socklen_t clientAddrSize = sizeof(clientAddr);

	char szEchoMessage[] = "Hello";

	SOCKET clientSock = 0;
	SSL*   pClientSslCtx = NULL;
	while (true) {
		while (true) {
			clientSock = accept(m_ServerSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
			if (clientSock == -1) {
				std::cout << __FUNCTION__ << " - Client accept() failed" << std::endl;
				continue;
			}

			std::cout << __FUNCTION__ << " - Client Accept: " << inet_ntoa(clientAddr.sin_addr) << std::endl;

			pClientSslCtx = SSL_new(m_pServerSslCtx);
			SSL_set_fd(pClientSslCtx, clientSock); 

			int retVal = SSL_accept(pClientSslCtx);
			std::cout << "SSL_accept(): " << retVal << std::endl;
			if (retVal <= 0) {
				std::cout << __FUNCTION__ << " - SSL_accept() failed" << std::endl;
				int errVal = SSL_get_error(pClientSslCtx, retVal);
				std::cout << "ERROR: " << errVal << std::endl;

				// Close
				SSL_free(pClientSslCtx);
#ifdef _WIN32
				closesocket(clientSock);
#elif __linux__
				close(clientSock);
#endif
				continue;
			}

			break;
		}
		
		char recvBuf[1025] = { 0, };
		if (SSL_read(pClientSslCtx, recvBuf, 1024) <= 0) {
			//ERR_print_errors_fp(stderr);
			std::cout << __FUNCTION__ << " - SSL_write() failed" << std::endl;
		}
		std::cout << recvBuf << std::endl;

		if (SSL_write(pClientSslCtx, szEchoMessage, strlen(szEchoMessage)) <= 0) {
			//ERR_print_errors_fp(stderr);
			std::cout << __FUNCTION__ << " - SSL_write() failed" << std::endl;			
		}

		// Close
		SSL_free(pClientSslCtx);
#ifdef _WIN32
		closesocket(clientSock);
#elif __linux__
		close(clientSock);
#endif
	}
}



SOCKET SSL_Server::CreateTcpSocket() {
	return CreateTcpSocket(DEFAULT_PORT);
}



SOCKET SSL_Server::CreateTcpSocket(int nPort) {
	SOCKET serverSock = 0;
	struct sockaddr_in sockAddr;

	memset(&sockAddr, 0x00, sizeof(sockAddr));
	sockAddr.sin_family = PF_INET;
	sockAddr.sin_port   = htons(nPort);
#ifdef _WIN32
	sockAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
#elif __linux__
	sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
#endif

	serverSock = socket(PF_INET, SOCK_STREAM, 0);
	if (serverSock == -1) {
		std::cout << __FUNCTION__ << " - socket() failed" << std::endl;
		return -1;
	}

	if (bind(serverSock, (struct sockaddr*)&sockAddr, sizeof(sockAddr)) == -1) {
		std::cout << __FUNCTION__ << " - bind() failed" << std::endl;
		return -1;
	}

	if (listen(serverSock, 5) == -1) {
		std::cout << __FUNCTION__ << " - listen() failed" << std::endl;
		return -1;
	}

	return serverSock;
}



SSL_CTX* SSL_Server::CreateSslCtx() {
	SSL_CTX* pSslCtx = NULL;

	pSslCtx = SSL_CTX_new(TLSv1_2_server_method());
	if (pSslCtx == NULL) {
		return NULL;
	}

	return pSslCtx;
}



SSL* SSL_Server::CreateClientCtx(SOCKET clientSock) {
	SSL* pSsl = NULL;

	pSsl = SSL_new(m_pServerSslCtx);
	if (pSsl == NULL) {
		return NULL;
	}

	SSL_set_fd(pSsl, clientSock);

	return pSsl;
}



bool SSL_Server::ConfigureContext() {
	return ConfigureContext(DEFAULT_CERT_FILE_PATH, DEFAULT_KEY_FILE_PATH);
}



bool SSL_Server::ConfigureContext(std::string strCertPath, std::string strKeyPem) {
	
	std::cout << strCertPath << std::endl;
	std::cout << strKeyPem   << std::endl;

	if (SSL_CTX_use_certificate_file(m_pServerSslCtx, strCertPath.c_str(), SSL_FILETYPE_PEM) == -1) {
		std::cout << __FUNCTION__ << " - SSL_CTX_use_certificate_file() failed" << std::endl;
		return false;
	}

	if (SSL_CTX_use_PrivateKey_file(m_pServerSslCtx, strKeyPem.c_str(), SSL_FILETYPE_PEM) == -1) {
		std::cout << __FUNCTION__ << " - SSL_CTX_use_PrivateKey_file() failed" << std::endl;
		return false;
	}

	if (SSL_CTX_check_private_key(m_pServerSslCtx) == -1)
	{
		std::cout << __FUNCTION__ << " - SSL 인증서 유효성 x" << std::endl;
		return false;
	}
	return true;
}

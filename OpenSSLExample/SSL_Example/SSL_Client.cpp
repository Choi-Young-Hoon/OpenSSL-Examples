#include "SSL_Client.h"

SSL_Client::SSL_Client()
: m_pSSL(NULL), m_pSSLCtx(NULL)
, m_Socket(0)
{}

SSL_Client::~SSL_Client() {
	if (m_pSSL != NULL) {
		SSL_free(m_pSSL);
	}

	if (m_pSSLCtx != NULL) {
		SSL_CTX_free(m_pSSLCtx);
	}
}



void SSL_Client::Initialize() {
	SSL_library_init();
	SSLeay_add_ssl_algorithms();
	//OpenSSL_add_ssl_algorithms();
	SSL_load_error_strings();
}



void SSL_Client::Finalize() {

}



SOCKET SSL_Client::CreateTcpSocket() {
	return CreateTcpSocket(DEFAULT_SERVER_PORT);
}



SOCKET SSL_Client::CreateTcpSocket(int nPort) {
	SOCKET sock;
	struct sockaddr_in sockAddr;

	memset(&sockAddr, 0x00, sizeof(sockAddr));
	sockAddr.sin_family = PF_INET;
	sockAddr.sin_port = htons(nPort);
#ifdef _WIN32
	sockAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
#elif __linux__
	sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
#endif

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		std::cout << __FUNCTION__ << " - socket() failed" << std::endl;
		return -1;
	}

	if (connect(sock, (struct sockaddr*)&sockAddr, sizeof(sockAddr)) == -1) {
		std::cout << __FUNCTION__ << " - connect() failed" << std::endl;
		return -1;
	}

	return sock;
}



SSL_CTX* SSL_Client::CreateSslCtx() {
	m_pSSLCtx = SSL_CTX_new(TLSv1_2_client_method());
	if (m_pSSLCtx == NULL) {
		std::cout << __FUNCTION__ << " - SSL_CTX_new() failed" << std::endl;
		return NULL;
	}

	m_pSSL = SSL_new(m_pSSLCtx);
	if (m_pSSL == NULL) {
		std::cout << __FUNCTION__ << " - SSL_new() failed" << std::endl;
		return NULL;
	}

	SSL_set_fd(m_pSSL, m_Socket);
	if (SSL_connect(m_pSSL) == -1) {
		std::cout << __FUNCTION__ << " - SSL_connect() failed" << std::endl;
		return NULL;
	}

	return m_pSSLCtx; 
}



void SSL_Client::Start() {
	Initialize();

	m_Socket = CreateTcpSocket();
	if (m_Socket == -1) {
		std::cout << "CreateTcpSocket() failed" << std::endl;
		return;
	}

	if (CreateSslCtx() == NULL) {
		std::cout << "CreateSslCtx() failed" << std::endl;
		return;
	}

	char buffer[1025] = { 0, };
	strcpy(buffer, "Hello");
	if (SSL_write(m_pSSL, buffer, strlen(buffer)) == -1) {
		std::cout << "SSL_write() failed" << std::endl;
		return ;
	}

	memset(buffer, 0x00, 1025);
	if (SSL_read(m_pSSL, buffer, 1024) == -1) {
		std::cout << "SSL_read() failed" << std::endl;
		return ;
	}

	Finalize();
	std::cout << buffer << std::endl;
}

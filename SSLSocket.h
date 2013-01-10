#ifndef SSLSOCKET_H
#define SSLSOCKET_H
#include <string>
#include "Socket.h"

/* OpenSSL headers */
#include "openssl/bio.h"
#include "openssl/ssl.h"
#include "openssl/err.h"

class SSLSocket : public Socket {
	public:
		SSLSocket(std::string hostname, int port);
		~SSLSocket();
		void send(const std::string message);
		int recv(std::string& buffer);
		int recv(std::string& buffer, const std::string& termination);
		int poll(int timeout_ms = 0);
	private:
		std::string m_hostname;
		int m_port;
		SSL_CTX * ctx;
		BIO * bio;
		SSL * ssl;
};
#endif /* SSLSOCKET_H */

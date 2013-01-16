#include "SSLSocket.h"
#include <iostream>
#include <stdlib.h>
#include <cstdio>
#include <sstream>

#define MAXDATASIZE 512 // max number of bytes we can get at once

SSLSocket::SSLSocket(std::string hostname, int port):
	m_hostname(hostname),
	m_port(port)
{
	SSL_load_error_strings();
	ERR_load_BIO_strings();
	OpenSSL_add_all_algorithms();
	SSL_library_init ();
	ctx = SSL_CTX_new(SSLv23_client_method());

	if(! SSL_CTX_load_verify_locations(ctx, NULL, "/etc/ssl/certs"))
	{
		std::cout << "Problem loading certificates" << std::endl;
		throw false;
	}
	std::ostringstream ost;
	ost << m_hostname << ":" << m_port;
	bio = BIO_new_ssl_connect(ctx);
	BIO_get_ssl(bio, & ssl);
	SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);
	BIO_set_conn_hostname(bio, ost.str().c_str());

	/* Verify the connection opened and perform the handshake */
	if(BIO_do_connect(bio) <= 0)
	{
		std::cout << "Problem connecting" << std::endl;
		throw false;
	}

	if(SSL_get_verify_result(ssl) != X509_V_OK)
	{
		std::cout << "Problem verifying" << std::endl;
		throw false;
	}
}

SSLSocket::~SSLSocket()
{
	BIO_reset(bio);
	SSL_CTX_free(ctx);
	BIO_free_all(bio);
}
void SSLSocket::send(const std::string message)
{
	while(BIO_write(bio, message.c_str(), strlen(message.c_str())) <= 0) {
		if(! BIO_should_retry(bio)) {
			throw false;
		}
	}
	return;
}

int SSLSocket::recv(std::string& buffer)
{
	buffer = "";
	char buf[MAXDATASIZE];

	int numbytes = BIO_read(bio, buf, MAXDATASIZE-1);
	if(numbytes == -1) {
		throw false;
	}
	for(int j = 0; j < numbytes; j++) {
		buffer += buf[j];
	}
	return numbytes;
}

int SSLSocket::recv(std::string& buffer, const std::string& termination)
{
	char buf;
	bool done = false;
	int numbytes_total = 0;
	buffer = "";
	while(!done) {
		if (BIO_read(bio, &buf, 1) == -1) {
			throw false;
		} else {
			numbytes_total++;
			buffer += buf;
			if(buffer.size() >= termination.size() && 
			   buffer.find(termination) != std::string::npos) {
				done = true;
				break;
			}
		}
	}
	return numbytes_total;
}

int SSLSocket::poll(int timeout_ms) {
	// TODO: FIX this...
	int ret = 0;
/*	int ret = -1;
	struct pollfd pfd;
	pfd.fd = m_sockfd;
	pfd.events = POLLIN;
	::poll(&pfd,1,timeout_ms);
	if(pfd.revents & POLLIN) {
		ret = 0;
	}*/
	return ret;
}

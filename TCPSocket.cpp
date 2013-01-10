#include "TCPSocket.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <arpa/inet.h>
#define MAXDATASIZE 512 // max number of bytes we can get at once
#include <sstream>
#include <iostream>
// get sockaddr, IPv4 or IPv6:

void* TCPSocket::get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

TCPSocket::TCPSocket(int* sockfd, std::string hostname, int port) :
	m_hostname(hostname),
	m_port(port),
	m_sockfd(sockfd)
{
}

TCPSocket::TCPSocket(std::string hostname, int port) :
	m_hostname(hostname),
	m_port(port),
	m_sockfd(new int)
{
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	std::ostringstream port_str;
	port_str << m_port;
	if ((rv = getaddrinfo(hostname.c_str(), port_str.str().c_str(), &hints, &servinfo)) != 0) {
		throw false;
	}

	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((*m_sockfd = socket(p->ai_family, p->ai_socktype,
			p->ai_protocol)) == -1) {
			continue;
		}
		printf("RegularSocket: Connecting to %s:%i\n",hostname.c_str(),m_port);
		if (connect(*m_sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(*m_sockfd);
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "RegularSocket: failed to connect\n");
		throw false;
	}

	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
		s, sizeof s);
	printf("RegularSocket: Connected to %s\n", hostname.c_str());

	freeaddrinfo(servinfo); // all done with this structure
}

TCPSocket::~TCPSocket()
{
	close(*m_sockfd);
	delete m_sockfd;
}

void TCPSocket::send(unsigned char* toSend, int numBytes) {
	if(::send(*m_sockfd,toSend,numBytes,0) == -1) {
		throw false;
	}
	return;
}

void TCPSocket::send(const std::string message)
{
	if(::send(*m_sockfd,message.c_str(),strlen(message.c_str()),0) == -1) {
		throw false;
	}
	return;
}

int TCPSocket::recv(std::string& buffer)
{
	buffer = "";
	char buf[MAXDATASIZE];

	int numbytes = ::recv(*m_sockfd, buf, MAXDATASIZE-1, 0);
	if(numbytes == -1) {
		throw false;
	}
	for(int j = 0; j < numbytes; j++) {
		buffer += buf[j];
	}
	return numbytes;
}

int TCPSocket::recv(std::string& buffer, int toReceive)
{
	buffer = "";
	char buf;
	int numbytes = 0;
	while(numbytes != toReceive) {
		numbytes += ::recv(*m_sockfd, &buf, 1, 0);
		buffer += buf;
		if(numbytes == -1) {
			throw false;
		}
	}
	return numbytes;
}

int TCPSocket::recv(std::string& buffer, const std::string& termination)
{
	char buf;
	bool done = false;
	int numbytes_total = 0;
	buffer = "";
	while(!done) {
		if (::recv(*m_sockfd, &buf, 1, 0) == -1) {
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

int TCPSocket::poll(int timeout_ms) {
	int ret = -1;
	struct pollfd pfd;
	pfd.fd = *m_sockfd;
	pfd.events = POLLIN;
	::poll(&pfd,1,timeout_ms);
	if(pfd.revents & POLLIN) {
		ret = 0;
	}
	return ret;
}

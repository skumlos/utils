#ifndef TCPSOCKET_H
#define TCPSOCKET_H
#include "Socket.h"

class TCPSocket : public Socket {
	public:
		TCPSocket(std::string hostname, int port);
		TCPSocket(int* sockfd, std::string hostname, int port);
		virtual ~TCPSocket();
		int getPort() { return m_port; };
		std::string getHostname() { return m_hostname; };
		void send(const std::string message);
		void send(unsigned char* toSend, int numBytes);
		int recv(std::string& buffer);
		int recv(std::string& buffer, int recvBytes);
		int recv(std::string& buffer, const std::string& termination);
		int peek();
		int poll(int timeout_ms = 0);
		bool isOpen();
	private:
		std::string m_hostname;
		int m_port;
		void* get_in_addr(struct sockaddr *sa);
		int *m_sockfd;
};

#endif /* TCPSOCKET_H */

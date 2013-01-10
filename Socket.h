#ifndef SOCKET_H
#define SOCKET_H
#include <string>

class Socket {
public:
	Socket() {};
	virtual ~Socket() {};
	virtual void send(const std::string message) = 0;
	virtual int recv(std::string& buffer) = 0;
	virtual int recv(std::string& buffer, const std::string& termination) = 0;
	virtual int poll(int timeout_ms = 0) = 0;
};

#endif /* SOCKET_H */

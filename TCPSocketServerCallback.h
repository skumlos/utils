/**
 * The class that wants to get the sockets from a TCPSocketServer should
 * inherit from this and implement the socketConnected method.
 *
 * 2013, Martin Hejnfelt (martin@hejnfelt.com)
 */

#ifndef TCPSOCKETSERVERCALLBACK_H
#define TCPSOCKETSERVERCALLBACK_H

#include "TCPSocket.h"

class TCPSocketServerCallback {
public:
	virtual void socketConnected(TCPSocket* newSocket) = 0;
};

#endif /* TCPSOCKETSERVERCALLBACK_H */


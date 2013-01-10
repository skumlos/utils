#ifndef TCPSOCKETSERVERCALLBACK_H
#define TCPSOCKETSERVERCALLBACK_H

#include "TCPSocket.h"

class TCPSocketServerCallback {
public:
	virtual void socketConnected(TCPSocket* newSocket) = 0;
};

#endif /* TCPSOCKETSERVERCALLBACK_H */


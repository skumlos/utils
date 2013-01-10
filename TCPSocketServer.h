#ifndef TCPSOCKETSERVER_H
#define TCPSOCKETSERVER_H
#include "Thread.h"
#include "Subject.h"
#include <list>

class TCPSocketServerCallback;

class TCPSocketServer : public Thread {
	public:
		TCPSocketServer(int port, TCPSocketServerCallback* callback);
		~TCPSocketServer();

		void thread();
	private:
		int m_sockfd;
		int m_port;
		TCPSocketServerCallback* m_callback;
};

#endif /* TCPSOCKETSERVER_H */

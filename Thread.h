#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>

class Thread {
public:
	Thread(bool deleteOnExit = false);
	virtual ~Thread();
	void start();
	void stop();
	virtual void doCleanup(){};
	bool isRunning() { return tid == (pthread_t)-1 ? false : true;};
	virtual void thread() = 0;
	static void cleanup(void*ptr);
protected:
	pthread_attr_t attr;
private:
	static void* _thread(void*ptr);
	bool m_deleteOnExit;
	pthread_t tid;
};

#endif /* THREAD_H */

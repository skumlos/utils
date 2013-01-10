#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>

class Thread {
public:
	Thread();
	~Thread();
	void start();
	void stop();
	bool isRunning() { return tid == (pthread_t)-1 ? false : true;};
	virtual void thread() = 0;
	static void cleanup(void*ptr);
protected:
	pthread_attr_t attr;
private:
	static void* _thread(void*ptr);
	pthread_t tid;
};

#endif /* THREAD_H */

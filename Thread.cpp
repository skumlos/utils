#include "Thread.h"
#include <cstdio>

Thread::Thread() :
	tid((pthread_t)-1)
{
	pthread_attr_init(&attr);
};

Thread::~Thread() {
	if(isRunning()) {
		stop();
	}
	pthread_attr_destroy(&attr);
};

void Thread::start() {
	if(tid == (pthread_t)-1) {
		pthread_create(&tid,&attr,Thread::_thread,(void*)this);
	}
};

void Thread::stop() {
	pthread_cancel(tid);
	pthread_join(tid,NULL);
};

void Thread::cleanup(void* ptr)
{
	Thread* t = (Thread*)ptr;
	t->stop();
	t->tid = (pthread_t)-1;
	t->doCleanup();
	delete t;
	return;
}

void* Thread::_thread(void*ptr) {
	pthread_cleanup_push(Thread::cleanup,ptr);
	Thread* self = (Thread*)ptr;
	self->thread();
	pthread_exit(0);
	pthread_cleanup_pop(1);
};


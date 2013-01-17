/**
 * Copyright (c) 2013, Martin Hejnfelt (martin@hejnfelt.com)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include "Thread.h"
#include <cstdio>

Thread::Thread(bool deleteOnExit) :
	tid((pthread_t)-1),
	m_deleteOnExit(deleteOnExit)
{
	pthread_attr_init(&attr);
};

Thread::~Thread() {
	if(tid != (pthread_t)-1) {
		if(isRunning()) {
			stop();
		}
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
	t->doCleanup();
	t->tid = (pthread_t)-1;
	if(t->m_deleteOnExit) {
		delete t;
	}
	return;
}

void* Thread::_thread(void*ptr) {
	pthread_cleanup_push(Thread::cleanup,ptr);
	Thread* self = (Thread*)ptr;
	self->thread();
	pthread_exit(0);
	pthread_cleanup_pop(1);
};


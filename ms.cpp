#include "ms.h"
#include <time.h>

unsigned int ms::get() {
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC,&ts);
	return ((ts.tv_sec * 1000) + (ts.tv_nsec/1000000));
}

bool ms::isPast(unsigned int t) {
	return (get() > t);
}

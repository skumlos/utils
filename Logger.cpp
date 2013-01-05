#include "Logger.h"
#include <pthread.h>
#include <cstdio>

Logger* Logger::instance = 0;
pthread_mutex_t Logger::instanceMutex = PTHREAD_MUTEX_INITIALIZER;

Logger::Logger()
{
}

Logger* Logger::getInstance()
{
	pthread_mutex_lock(&instanceMutex);
	if(instance == 0) {
		instance = new Logger();
	}
	return instance;
	pthread_mutex_unlock(&instanceMutex);
}

void Logger::log(const LogEntry& entry)
{
	std::printf("Logger - %s: %s\n",entry.origin.c_str(),entry.message.c_str()); 
}

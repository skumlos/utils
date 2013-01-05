#ifndef LOGGER_H
#define LOGGER_H
#include <string>

class Logger {
	public:
		typedef struct {
			std::string origin;
			std::string message;
		} LogEntry;

		static Logger* getInstance();
		void log(const LogEntry& entry);
	private:
		static pthread_mutex_t instanceMutex;
		static Logger* instance;
		Logger();
};

#endif /* LOGGER_H */
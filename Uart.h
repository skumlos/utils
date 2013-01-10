#ifndef UART_H
#define UART_H
#include <stdint.h>
#include <termios.h>
#include <string>
#include <vector>
#include <pthread.h>

class UARTException {
	public:
		UARTException(std::string reason) : m_reason(reason) {};
		std::string getReason() { return m_reason; };
	private:
		std::string m_reason;
};

class UART {
	public:
		UART(std::string tty, bool hwflowctrl = false) throw (UARTException);
		~UART();
		void lock(std::string locker = "");
		void unlock(std::string unlocker = "");
		void setSpeed(speed_t new_speed) throw (UARTException);
		void write(const std::string& msg) throw (UARTException);
		void write(const std::vector<unsigned char>& msg) throw (UARTException);
		int read(std::string& buf) throw (UARTException);
		unsigned char readByte() throw (UARTException);
		void flush();
		int poll(int timeout_ms = -1);
	private:
		int m_hwflowctrl;
		void set_8N1() throw (UARTException);
		int fd;
		std::string m_tty;
		pthread_mutex_t m_portmutex;
};

#endif /* UART_H */

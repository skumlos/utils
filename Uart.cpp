#include "Uart.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <sys/poll.h>
#include <iostream>

UART::UART(std::string tty, bool hwflowctrl) throw (UARTException) :
	m_tty(tty),
	fd(0),
	m_hwflowctrl(hwflowctrl)
{
	fd = open(tty.c_str(), O_RDWR|O_NOCTTY);
	if(fd < 0) {
		throw UARTException("Could not open file.");
	}
	try {
		set_8N1();
	} catch (UARTException ex) {
		throw ex;
	}
	pthread_mutex_init(&m_portmutex,0);
	std::cout << "Opened " << tty << std::endl;
}

UART::~UART()
{
	pthread_mutex_destroy(&m_portmutex);
}

void UART::flush()
{
	tcflush(fd,TCIOFLUSH);
}

int UART::poll(int timeout_ms)
{
	struct pollfd pfd = {fd, POLLIN, 0};
	int err;

        err = ::poll(&pfd, 1, timeout_ms);
        if (err == -1) {
                perror("UART: poll failed");
                return -1;
        }
	return err;
}

void UART::setSpeed(speed_t new_speed) throw (UARTException)
{
	struct termios tnew;
        int err;

	tcgetattr(fd, &tnew);

        /* speed settings */
        cfsetispeed(&tnew, new_speed);
        cfsetospeed(&tnew, new_speed);

	err =  tcsetattr(fd, TCSAFLUSH, &tnew);
        if (err) {
                throw UARTException("Could not set speed");
        }
	return;
}

void UART::set_8N1() throw (UARTException)
{
	struct termios tnew;
        int err;

        err = tcgetattr(fd, &tnew);
        if (err) {
		printf("C %d\n",errno);
		throw new UARTException("Could not get port attributes");
        }

        tnew.c_cflag |= (CLOCAL | CREAD);

        tnew.c_cflag &= ~PARENB;
        tnew.c_cflag &= ~CSTOPB;
        tnew.c_cflag &= ~CSIZE;
        tnew.c_cflag |=  CS8;

	if(m_hwflowctrl) {
		tnew.c_cflag |= CRTSCTS;
	} else {
		tnew.c_cflag &= ~CRTSCTS;
	}

        tnew.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
        tnew.c_iflag &= ~(IXON | IXOFF | IXANY);
        tnew.c_iflag &= ~(ICRNL | INLCR);
        tnew.c_oflag &= ~OPOST;
        tnew.c_cc[VTIME] = 0;
        tnew.c_cc[VMIN]  = 1;

        err = tcsetattr(fd, TCSAFLUSH, &tnew);
        if (err) {
		throw UARTException("Could not set port attributes");
        }
	return;
}

void UART::lock(std::string locker)
{
//	std::cout << "lock by " << locker << std::endl;
	pthread_mutex_lock(&m_portmutex);
//	std::cout << "locked by " << locker << std::endl;
}

void UART::unlock(std::string unlocker)
{
//	std::cout << "unlock by " << unlocker << std::endl;
	pthread_mutex_unlock(&m_portmutex);
//	std::cout << "unlocked by " << unlocker << std::endl;
}

unsigned char UART::readByte() throw (UARTException) {
	unsigned char c = 0;
	if(::read(fd,&c,1) < 0) {
		throw new UARTException("readByte: No data available.");
	}
	return c;
}

int UART::read(std::string& buf) throw (UARTException)
{
	char _buf;
	while(::read(fd,&_buf,1) > 0) {
		buf += _buf;
	}
	return 0;
}

void UART::write(const std::string& msg) throw (UARTException)
{
	flush();
	int written = ::write(fd,msg.c_str(),msg.size());
	if(written != msg.size()) {
		throw new UARTException("Problem sending all data");
	}
	return;
}

void UART::write(const std::vector<unsigned char>& msg) throw (UARTException)
{
	flush();
	int written = 0;
	for(unsigned int j = 0; j < msg.size(); j++) {
		written += ::write(fd,&msg[j],1);
	}
	if(written != msg.size()) {
		throw new UARTException("Problem sending all data");
	}
	return;
}

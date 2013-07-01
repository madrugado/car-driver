#ifndef CAR_DRIVER_H_INCLUDED
#define CAR_DRIVER_H_INCLUDED

#include <stdint.h>
#include <limits.h>

#include "serial-port.h"
#include "commands.h"

#define BUF_SIZE 64

int setPWM(int fd, int ch_A, int ch_B);
int resetWTCTimer(int fd);

int setPWM(int fd, int ch_A, int ch_B)
{
  unsigned char buf[BUF_SIZE];

  buf[0] = 5;
  buf[1] = deviceCommand::SET_DUTY_CYCLE;
  buf[2] = (uint8_t)(ch_A & 0xFF);
  buf[3] = (uint8_t)(ch_A >> 8);
  buf[4] = (uint8_t)(ch_B & 0xFF);
  buf[5] = (uint8_t)(ch_B >> 8);

  serialPort::send_to_device(fd, buf, (2 + 2 * sizeof(uint16_t)));

  return 0;
}

int resetWTCTimer(int fd)
{
  unsigned char buf[BUF_SIZE];

  buf[0] = 1;
  buf[1] = deviceCommand::RESET_WFC_TIMER;

  serialPort::send_to_device(fd, buf, 2);
  return 0;
}

const unsigned long timeout = 10000000;

bool timeoutExeeded(struct timeval* recent, struct timeval* previous)
{
	__time_t secDiff = recent->tv_sec - previous->tv_sec;
	if (secDiff < 0)
		return false;
	if (secDiff >= ULONG_MAX/1000000)
		return true;
	else
	{
		long diff = secDiff * 1000000 + recent->tv_usec - previous->tv_usec;
		if (diff < 0)
			return false;
		if (diff > timeout)
			return true;
		else
			return false;
	}
}

#endif //CAR_DRIVER_H_INCLUDED

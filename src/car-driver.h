#ifndef CAR_DRIVER_H_INCLUDED
#define CAR_DRIVER_H_INCLUDED

#include <stdint.h>
#include <limits.h>

#include "serial-port.h"
#include "commands.h"

#define BUF_SIZE 64

int setPWM(int fd, int ch_A, int ch_B);
int resetWTCTimer(int fd);
int record(const char* filename);

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

int getDistanceFromSonar(int fd, double &distance)
  {
    uint8_t cmdBuf[sizeof(uint8_t) * 2];

    cmdBuf[0] = sizeof(uint8_t);
    cmdBuf[1] = deviceCommand::GET_SONAR_IL;

    uint8_t inBuf[IN_BUF_SIZE];

    int resCode;

    resCode = serialPort::send_to_device(fd, cmdBuf, sizeof(char) * 2);
    if (resCode != 0)
      return -2;

    resCode = serialPort::read_from_device(fd, inBuf, sizeof(uint16_t));
    if (resCode != 0)
      return -2;

    distance = static_cast<double>(inBuf[0] + (inBuf[1] << 8));
    distance /= FRONT_SONAR_COEFFICIENT;

    return 0;
  }

__suseconds_t timeDiff(struct timeval& tv1, struct timeval& tv2)
{
  if ((tv1.tv_sec == 0 && tv1.tv_usec == 0) || (tv2.tv_sec == 0 && tv2.tv_usec == 0))
    return 0;
  return (tv2.tv_sec - tv1.tv_sec) * 1000000 + tv2.tv_usec - tv1.tv_usec;
}

#endif //CAR_DRIVER_H_INCLUDED

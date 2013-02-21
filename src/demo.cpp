#include <cstdio>
#include <cstdlib>
#include <unistd.h>

#include "serial-port.h"
#include "car-driver.h"

#define PORT_NAME "/dev/ttyUSB0"

int main(int argc, char** argv)
{
  int fd = serialPort::open_port(PORT_NAME);
  if (fd == -1)
    return 1;
  serialPort::configure_port(fd);

  // turn left
  setPWM(fd, 1500, 1060);
  usleep(250000);
  resetWTCTimer(fd);
  usleep(250000);

  // turn right
  setPWM(fd, 1500, 1850);
  usleep(250000);
  resetWTCTimer(fd);
  usleep(250000);

  // move forward slowly
  setPWM(fd, 1650, 1450);
  usleep(250000);
  resetWTCTimer(fd);
  usleep(250000);

  // move forward quickly
  setPWM(fd, 2000, 1450);
  usleep(100000);

  // braking
  setPWM(fd, 1000, 1450);
  usleep(250000);
  resetWTCTimer(fd);
  usleep(250000);
  
  // move backward after braking
  setPWM(fd, 1500, 1450); // stop braking
  usleep(250000);
  setPWM(fd, 1350, 1450); // move backward
  usleep(250000);
  resetWTCTimer(fd);
  usleep(250000);

  // set defaults
  setPWM(fd, 1500, 1450);

  serialPort::close_port(fd);
  return 0;
}

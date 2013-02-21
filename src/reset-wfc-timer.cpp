#include <cstdio>
#include <cstdlib>

#include "serial-port.h"
#include "car-driver.h"

#define PORT_NAME "/dev/ttyUSB0"

int main(int argc, char** argv)
{
  int fd = serialPort::open_port(PORT_NAME);
  if (fd == -1)
    return 1;
  serialPort::configure_port(fd);

  resetWTCTimer(fd);

  serialPort::close_port(fd);
  return 0;
}

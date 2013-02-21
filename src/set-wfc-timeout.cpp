#include <cstdio>
#include <cstdlib>
#include <stdint.h>

#include "serial-port.h"
#include "car-driver.h"

#define PORT_NAME "/dev/ttyUSB0"
#define BUF_SIZE 64

int main(int argc, char** argv)
{
  if (argc != 2)
  {  
    printf("Usage: <timeout> in milliseconds (multiple of 16 ms)\n");
    return 1;
  }

  int wfc_timeout = atoi(argv[1]);
  if (wfc_timeout < 0 || wfc_timeout > 0xFFFF)
  {
    fprintf(stderr, "Error: bad args\n");
    return 1;
  }

  unsigned char buf[BUF_SIZE];

  buf[0] = 3;
  buf[1] = deviceCommand::SET_WFC_TIMEOUT;
  buf[2] = (uint8_t)(wfc_timeout & 0xFF);
  buf[3] = (uint8_t)(wfc_timeout >> 8);

  int fd = serialPort::open_port(PORT_NAME);
  if (fd == -1)
    return 1;
  serialPort::configure_port(fd);
  serialPort::send_to_device(fd, buf, (2 + sizeof(uint16_t)));
  serialPort::close_port(fd);

  return 0;
}

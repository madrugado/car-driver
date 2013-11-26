#include <cstdio>
#include <cstdlib>

#include "serial-port.h"
#include "car-driver.h"

#define PORT_NAME "/dev/ttyACM0"

int main(int argc, char** argv)
{
  if (argc != 3)
  {  
    printf("Usage: <pwm1_duty> <pwm2_duty>\n");
    return 1;
  }

  int ch_A = atoi(argv[1]);
  int ch_B = atoi(argv[2]);
  if (ch_A < 0 || ch_B < 0 || ch_A > 0xFFFF || ch_B > 0xFFFF)
  {
    fprintf(stderr, "Error: bad args\n");
    return 1;
  }

  int fd = serialPort::open_port(PORT_NAME);
  if (fd == -1)
    return 1;
  serialPort::configure_port(fd);

  setPWM(fd, ch_A, ch_B);

  serialPort::close_port(fd);
  return 0;
}

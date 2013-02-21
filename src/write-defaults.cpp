#include <cstdio>
#include <cstdlib>
#include <stdint.h>
#include <unistd.h>

#include "serial-port.h"
#include "commands.h"

#define PORT_NAME "/dev/ttyUSB0"
#define BUF_SIZE 64

int main(int argc, char** argv)
{
  if (argc != 8)
  {  
    printf("Usage: <ICR> <OCRA> <CH_A_MAX> <CH_A_MIN> <OCRB> <CH_B_MAX> <CH_B_MIN>\n");
    return 1;
  }
  
  int _ICR      = atoi(argv[1]);
  int _OCRA     = atoi(argv[2]);
  int _CH_A_MAX = atoi(argv[3]);
  int _CH_A_MIN = atoi(argv[4]);
  int _OCRB     = atoi(argv[5]);
  int _CH_B_MAX = atoi(argv[6]);
  int _CH_B_MIN = atoi(argv[7]);

  unsigned char buf[BUF_SIZE];

  buf[0]  = 15;
  buf[1]  = deviceCommand::SET_DEFAULTS;
  buf[2]  = (uint8_t)(_ICR & 0xFF);
  buf[3]  = (uint8_t)(_ICR >> 8);
  buf[4]  = (uint8_t)(_OCRA & 0xFF);
  buf[5]  = (uint8_t)(_OCRA >> 8);
  buf[6]  = (uint8_t)(_CH_A_MAX & 0xFF);
  buf[7]  = (uint8_t)(_CH_A_MAX >> 8);
  buf[8]  = (uint8_t)(_CH_A_MIN & 0xFF);
  buf[9]  = (uint8_t)(_CH_A_MIN >> 8);
  buf[10] = (uint8_t)(_OCRB & 0xFF);
  buf[11] = (uint8_t)(_OCRB >> 8);
  buf[12] = (uint8_t)(_CH_B_MAX & 0xFF);
  buf[13] = (uint8_t)(_CH_B_MAX >> 8);
  buf[14] = (uint8_t)(_CH_B_MIN & 0xFF);
  buf[15] = (uint8_t)(_CH_B_MIN >> 8);

  int fd = serialPort::open_port(PORT_NAME);
  if (fd == -1)
    return 1;
  serialPort::configure_port(fd);
  serialPort::send_to_device(fd, buf, (2 + 7 * sizeof(uint16_t)));
  serialPort::close_port(fd);

  return 0;
}

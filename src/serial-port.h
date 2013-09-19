#ifndef SERIAL_PORT_H_INCLUDED
#define SERIAL_PORT_H_INCLUDED

#include <stdio.h>   // standard input / output functions
#include <string.h>  // string function definitions
#include <unistd.h>  // UNIX standard function definitions
#include <fcntl.h>   // File control definitions
#include <errno.h>   // Error number definitions
#include <termios.h> // POSIX terminal control definitions
#include <time.h>    // time calls

#define TIMEOUT_S  1
#define TIMEOUT_US 500000

#define IN_BUF_SIZE 64

namespace serialPort
{
  //const char Preamble[] = {'<', '<', 'U', 'U', 'S', 'S', 'w', '<', '<', 'U', 'S', 'S', 'w', 'w', '<', '<'};
  const char Preamble[] = {'<', 'U', 'S', 'w'};
  
  int open_port(const char *port_name);
  int configure_port(int fd);
  int send_to_device(int fd, const unsigned char* send_bytes, int size);
  int close_port(int fd);

  int open_port(const char *port_name)
  {
    int fd; // file description for the serial port
  
    fd = open(port_name, O_RDWR | O_NOCTTY | O_NDELAY);
  
    if(fd == -1) // if open is unsucessful
    {
      printf("SerialPort: Error: unable to open %s\n", port_name);
    }
    else
    {
      fcntl(fd, F_SETFL, 0);
      printf("SerialPort: port is open\n");
    }
  
    return(fd);
  }

  int configure_port(int fd)
  {
    struct termios port_settings;        // structure to store the port settings in

    cfsetispeed(&port_settings, B38400); // set baud rates
    cfsetospeed(&port_settings, B38400);

    port_settings.c_cflag &= ~PARENB;    // set no parity, stop bits, data bits
    port_settings.c_cflag &= ~CSTOPB;
    port_settings.c_cflag &= ~CSIZE;
    port_settings.c_cflag |= CS8;

    tcsetattr(fd, TCSANOW, &port_settings); // apply the settings to the port
    return(fd);
  }

  int send_to_device(int fd, const unsigned char* send_bytes, int size)
  {
    char n;
    fd_set rdfs;
    struct timeval timeout;
    char inBuf[IN_BUF_SIZE];
  
    timeout.tv_sec = TIMEOUT_S;
    timeout.tv_usec = TIMEOUT_US;
    
    char* msg = (char*) malloc(size + sizeof(Preamble));
    memcpy(msg, Preamble, sizeof(Preamble));
    memcpy(msg + sizeof(Preamble), send_bytes, size);
  
    int bytes_sent = write(fd, msg, size + sizeof(Preamble));  // send data

    //n = select(fd + 1, &rdfs, NULL, NULL, &timeout);
    //if(n < 0)
    //{
    //  perror("SerialPort: Error: select failed\n");
    //}
    //else if (n == 0)
    //{
    //  printf("Timeout!\n");
    //}
    //else
    //{
    //  int r = read(fd, inBuf, 2);
    //  inBuf[1] = 0;
    //  if(r == -1)
    //  {
    //    fprintf(stderr, "SerialPort: Error: read returned -1\n");
    //  }
    //  else
    //    printf("SerialPort: received value: %s\n", inBuf);
    //}

    return 0;
  }

  int close_port(int fd)
  {
    close(fd);
    printf("SerialPort: port is closed\n");
    return 0;
  }

} //namespace serialPort

#endif //SERIAL_PORT_H_INCLUDED

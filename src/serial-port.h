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

#define FRONT_SONAR_COEFFICIENT 200.0

namespace serialPort
{
  //const char Preamble[] = {'<', '<', 'U', 'U', 'S', 'S', 'w', '<', '<', 'U', 'S', 'S', 'w', 'w', '<', '<'};
  const char Preamble[] = {'<', 'U', 'S', 'w'};
  
  int open_port(const char *port_name);
  int configure_port(int fd);
  int send_to_device(int fd, const unsigned char* send_bytes, int size);
  int read_from_device(int fd, unsigned char* received_bytes, int size);
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

  int read_from_device(int fd, unsigned char* received_bytes, int size)
  {
    struct timeval timeout;
    timeout.tv_sec = TIMEOUT_S;
    timeout.tv_usec = TIMEOUT_US;

    fd_set rdfs;
    FD_ZERO(&rdfs);
    FD_SET(fd, &rdfs);

    char inBuf[IN_BUF_SIZE];
    char n;

    n = select(fd + 1, &rdfs, NULL, NULL, &timeout);
    if(n < 0) {
      perror("serialPort: Error: select() failed\n");
      return -1;
    }
    else if (n == 0) {
      printf("serialPort: Error: read timeout\n");
      return -1;
    }
    else {
      int totalSize = sizeof(Preamble) + 1 + size; //<preamble> + <size> + <data>
      int resCode = read(fd, inBuf, totalSize);
      if(resCode == -1) {
        fprintf(stderr, "serialPort: Error: read returned -1\n");
      }
      else
        for (int i = 0; i < size; i++)
          received_bytes[i] = inBuf[i + sizeof(Preamble) + 1];

          // debug
          /*
          int b0 = inBuf[sizeof(Preamble) + 1];
          int b1 = inBuf[sizeof(Preamble) + 2];
          printf("serialPort: received values: %d %d\n", b0, b1);
          */
          /*
          for (int i = 0; i < totalSize; i++)
            printf("serialPort: received: i = %d, v = %d\n", i, inBuf[i]);
          */
    }
    return 0;
  }

  int getDistanceFromSonar(int fd, double &distance)
  {
    uint8_t cmdBuf[sizeof(uint8_t) * 2];

    cmdBuf[0] = sizeof(uint8_t);
    cmdBuf[1] = deviceCommand::GET_SONAR_IL;

    uint8_t inBuf[IN_BUF_SIZE];

    int resCode;

    resCode = send_to_device(fd, cmdBuf, sizeof(char) * 2);
    if (resCode != 0)
      return -2;

    resCode = read_from_device(fd, inBuf, sizeof(uint16_t));
    if (resCode != 0)
      return -2;

    distance = static_cast<double>(inBuf[0] + (inBuf[1] << 8));
    distance /= FRONT_SONAR_COEFFICIENT;

    return 0;
  }

} //namespace serialPort

#endif //SERIAL_PORT_H_INCLUDED

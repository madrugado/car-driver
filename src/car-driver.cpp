//============================================================================
// Name        : kbd-reader.cpp
// Author      : Val Malykh
// Version     : 0.1
// Copyright   : BSD 2-clause license
// Description : Keyboard reading program for VisiRoad project
//============================================================================

#include <sys/time.h>

#include <cstdio>
#include <cstdlib>
#include <unistd.h>

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


#include "serial-port.h"
#include "car-driver.h"

#define PORT_NAME "/dev/ttyUSB0"

#define A_MAX 2000
#define A_MIN 1000
#define A_NTR 1500

#define B_MAX 1850
#define B_MIN 1060
#define B_NTR 1450

typedef	unsigned short	WORD;
#define	XINPUT_GAMEPAD_DPAD_UP		0x0001
#define	XINPUT_GAMEPAD_DPAD_DOWN	0x0002
#define	XINPUT_GAMEPAD_DPAD_LEFT	0x0004
#define	XINPUT_GAMEPAD_DPAD_RIGHT	0x0008
#define	XINPUT_GAMEPAD_START		0x0010

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

unsigned channelA = A_NTR, channelB = B_NTR; 
suseconds_t prevA = 0, prevB = 0;
const unsigned stepA = 10;
const unsigned stepB = 10;
const unsigned long timeout = 500000;

void renewA(struct timeval *tv, bool forward)
{
	if (forward)
		if ((tv->tv_usec - prevA) > timeout)
				channelA = A_NTR + stepA;
		else
		{
			if (channelA < A_MAX - stepA)
				channelA += stepA;
		}
	else
		if ((tv->tv_usec - prevA) > timeout)
			channelA = A_NTR - stepA;
		else
			if (channelA > A_MIN + stepA)
				channelA -= stepA;
	
	
	prevA = tv->tv_usec;
}

void renewB(struct timeval *tv, bool right)
{
	if (right)
		if ((tv->tv_usec - prevB) > timeout)
				channelB = B_NTR + stepB;
		else
		{
			if (channelB < B_MAX - stepB)
				channelB += stepB;
		}
	else
		if ((tv->tv_usec - prevB) > timeout)
			channelB = B_NTR - stepB;
		else
			if (channelB > B_MIN + stepB)
				channelB -= stepB;


	prevB = tv->tv_usec;
}

int main(int argc, char** argv)
{
    int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd,
                 (struct sockaddr *) &cli_addr,
                 &clilen);
     if (newsockfd < 0)
          error("ERROR on accept");
     bzero(buffer,256);

	  int fd = serialPort::open_port(PORT_NAME);
	  if (fd == -1)
	    return 1;
	  serialPort::configure_port(fd);

	struct timeval tv;


	while (true)
	{
		 n = read(newsockfd, buffer, sizeof(WORD));
		 if (n < 0) error("ERROR reading from socket");
		 if ( buffer[0] & XINPUT_GAMEPAD_START)
		{
			puts("Received START. Exiting.");
			 break;
		}
		 printf("Here is the message: %04x\n", (buffer[1] << 8) | buffer[0]);
		gettimeofday(&tv, NULL);

		 if ( buffer[0] & XINPUT_GAMEPAD_DPAD_DOWN)
		 {
				printf("DOWN Key pressed\n");
				renewA(&tv, false);
				printf("Now A is %d", channelA);
		 }
		 if ( buffer[0] & XINPUT_GAMEPAD_DPAD_UP)
		 {
				printf("UP Key pressed\n");
				renewA(&tv, true);
				printf("Now A is %d", channelA);
		 }
		 if ( buffer[0] & XINPUT_GAMEPAD_DPAD_LEFT)
		 {
				printf("LEFT Key pressed\n");
				renewB(&tv, false);
				printf("Now B is %d", channelB);
		 }
		 if ( buffer[0] & XINPUT_GAMEPAD_DPAD_RIGHT)
		 {
				printf("RIGHT Key pressed\n");
				renewB(&tv, true);
				printf("Now B is %d", channelB);
		 }
		setPWM(fd, channelA, channelB);
	}


    n = write(newsockfd,"I got your message",18);
    if (n < 0) error("ERROR writing to socket");
    close(newsockfd);
    close(sockfd);

	  // set defaults
	  setPWM(fd, 1500, 1450);

	  serialPort::close_port(fd);

	return 0;
}

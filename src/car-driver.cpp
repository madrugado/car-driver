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
#include <linux/tcp.h>

#include "serial-port.h"
#include "car-driver.h"

#define PORT_NAME "/dev/ttyUSB0"

#define A_MAX 1850
#define A_MIN 1060
#define A_NTR 1450

#define B_MAX 2000
#define B_MIN 1000
#define B_NTR 1470

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
const unsigned stepA = 50;
const unsigned stepB = 30;
const unsigned long timeout = 10000000;

void renewA(struct timeval *tv, bool right)
{
	if (right)
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

void renewB(struct timeval *tv, bool forward)
{
	if (forward)
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
	
	 int flag = 1;
         int result = setsockopt(sockfd,            /* socket affected */
                                 IPPROTO_TCP,     /* set option at TCP level */
                                 TCP_NODELAY,     /* name of option */
                                 (char *) &flag,  /* the cast is historical
                                                         cruft */
                                 sizeof(int));    /* length of option value */
         if (result < 0)
		error("ERROR on setting TCP_NODELAY option");

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
	suseconds_t prevT = 0;


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
				renewB(&tv, false);
				printf("Now B is %d\n", channelB);
		 }
		 if ( buffer[0] & XINPUT_GAMEPAD_DPAD_UP)
		 {
				printf("UP Key pressed\n");
				renewB(&tv, true);
				printf("Now B is %d\n", channelB);
		 }
		 if ( buffer[0] & XINPUT_GAMEPAD_DPAD_LEFT)
		 {
				printf("LEFT Key pressed\n");
				renewA(&tv, false);
				printf("Now A is %d\n", channelA);
		 }
		 if ( buffer[0] & XINPUT_GAMEPAD_DPAD_RIGHT)
		 {
				printf("RIGHT Key pressed\n");
				renewA(&tv, true);
				printf("Now A is %d\n", channelA);
		 }
		//if (tv.tv_usec - prevT > 300000)
			setPWM(fd, channelA, channelB);
		prevT = tv.tv_usec;
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

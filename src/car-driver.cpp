//============================================================================
// Name        : kbd-reader.cpp
// Author      : Val Malykh
// Version     : 0.1
// Copyright   : BSD 2-clause license
// Description : Keyboard reading program for VisiRoad project
//============================================================================

#include <ncurses.h>
#include <sys/time.h>

#include <cstdio>
#include <cstdlib>
#include <unistd.h>

#include "serial-port.h"
#include "car-driver.h"

#define PORT_NAME "/dev/ttyUSB0"

#define A_MAX 2000
#define A_MIN 1000
#define A_NTR 1500

#define B_MAX 1850
#define B_MIN 1060
#define B_NTR 1450

unsigned channelA = A_NTR, channelB = B_NTR; 
suseconds_t prevA = 0, prevB = 0;

void renewA(struct timeval *tv, bool forward)
{
	if (forward)
		if ((prevA - tv->tv_usec) > 500)
				channelA = A_NTR + 10;
		else
		{
			if (channelA < A_MAX - 10)
				channelA += 10;
		}
	else
		if ((prevA - tv->tv_usec) > 500)
			channelA = A_NTR - 10;
		else
			if (channelA > A_MIN + 10)
				channelA -= 10;
	
	
	prevA = tv->tv_usec;
}

void renewB(struct timeval *tv, bool right)
{
	if (right)
		if ((prevB - tv->tv_usec) > 500)
				channelB = B_NTR + 10;
		else
		{
			if (channelB < B_MAX - 10)
				channelB += 10;
		}
	else
		if ((prevB - tv->tv_usec) > 500)
			channelB = B_NTR - 10;
		else
			if (channelB > B_MIN + 10)
				channelB -= 10;


	prevB = tv->tv_usec;
}

int main()
{
	  int fd = serialPort::open_port(PORT_NAME);
	  if (fd == -1)
	    return 1;
	  serialPort::configure_port(fd);

	int ch;

	struct timeval tv;

	initscr();
	raw();
	keypad(stdscr, TRUE);
	noecho();

	ch = getch();
	gettimeofday(&tv, NULL);

	while (ch != 27)
	{
		switch (ch)
			{
			case KEY_DOWN:
				mvprintw(12,30,"DOWN Key pressed\n");
				renewA(&tv, false);
				printw("Now A is %d", channelA);
				break;
			case KEY_UP:
				mvprintw(12,30,"UP Key pressed\n");
				renewA(&tv, true);
				printw("Now A is %d", channelA);
				break;
			case KEY_LEFT:
				mvprintw(12,30,"LEFT Key pressed\n");
				renewB(&tv, false);
				printw("Now B is %d", channelB);
				break;
			case KEY_RIGHT:
				mvprintw(12,30,"RIGHT Key pressed\n");
				renewB(&tv, true);
				printw("Now B is %d", channelB);
				break;
			default:
				break;
			}
		refresh();
		setPWM(fd, channelA, channelB);
		ch = getch();
		gettimeofday(&tv, NULL);
	}

	endwin();

	  // set defaults
	  setPWM(fd, 1500, 1450);

	  serialPort::close_port(fd);

	return 0;
}

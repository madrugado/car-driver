###############################################################
#
# Purpose: Makefile for "car-driver"
# Author.: Val Malykh 
# Version: 0.1
# License: BSD
#
###############################################################

# specifies where to install the binaries after compilation
# to use another directory you can specify it with:
# $ sudo make DESTDIR=/some/path install
DESTDIR = .

# set the compiler to use
CC = g++


# general compile flags, enable all warnings to make compile more verbose
CFLAGS += -O2 -DLINUX -D_GNU_SOURCE -Wall 
# CFLAGS += -g 
# CFLAGS +=  -DDEBUG

# LFLAGS += -lncurses

SRCDIR = ./src

APPS = car-driver demo

# this is the first target, thus it will be used implictely if no other target
# was given. It defines that it is dependent on the application target and
# the plugins
all: $(APPS)

car-driver: ${SRCDIR}/car-driver.cpp ${SRCDIR}/commands.h ${SRCDIR}/car-driver.h
	$(CC) $(CFLAGS) ${SRCDIR}/car-driver.cpp $(LFLAGS) -o car-driver
	chmod 755 car-driver

demo: ${SRCDIR}/demo.cpp ${SRCDIR}/commands.h ${SRCDIR}/car-driver.h
	$(CC) $(CFLAGS) ${SRCDIR}/demo.cpp $(LFLAGS) -o demo
	chmod 755 demo	

# cleanup
clean:
	rm -f *.a *.o $(APPS) core *~ *.so *.lo


# install MJPG-streamer and example webpages
install: all
	for app in $(APPS); do \
		install --mode=755 $$app $(DESTDIR) ; \
	done;

# remove the files installed above
uninstall:
	for app in $(APPS); do \
		rm -f $(DESTDIR)/$$app ; \
	done;

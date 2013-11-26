###############################################################
#
# Purpose: Makefile for "car-driver"
# Author.: Val Malykh 
# Version: 0.2
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
CFLAGS += -DLINUX -D_GNU_SOURCE -Wall 
# CFLAGS += -g 
# CFLAGS +=  -DDEBUG

# LFLAGS += -lncurses

SRCDIR = ./src

APPS = car-driver demo

UTILS = write-defaults pwm-test

# this is the first target, thus it will be used implictely if no other target
# was given. It defines that it is dependent on the application target and
# the plugins
all: apps utils

apps: $(APPS)

car-driver: ${SRCDIR}/car-driver.cpp ${SRCDIR}/commands.h ${SRCDIR}/car-driver.h
	$(CC) $(CFLAGS) ${SRCDIR}/car-driver.cpp $(LFLAGS) -o car-driver
	chmod 755 car-driver

demo: ${SRCDIR}/demo.cpp ${SRCDIR}/commands.h ${SRCDIR}/car-driver.h
	$(CC) $(CFLAGS) ${SRCDIR}/demo.cpp $(LFLAGS) -o demo
	chmod 755 demo	

utils: $(UTILS)

write-defaults: ${SRCDIR}/write-defaults.cpp ${SRCDIR}/commands.h ${SRCDIR}/serial-port.h
		$(CC) $(CFLAGS) ${SRCDIR}/write-defaults.cpp $(LFLAGS) -o utils/write-defaults
		chmod 755 utils/write-defaults

pwm-test: ${SRCDIR}/pwm-test.cpp ${SRCDIR}/commands.h ${SRCDIR}/car-driver.h
		$(CC) $(CFLAGS) ${SRCDIR}/pwm-test.cpp $(LFLAGS) -o utils/pwm-test
		chmod 755 utils/pwm-test

# cleanup
clean:
	rm -f *.a *.o $(APPS) core *~ *.so *.lo
	for util in $(UTILS); do \
		rm -f utils/$$util ; \
        done;



# install
install: all
	for app in $(APPS); do \
		install --mode=755 $$app $(DESTDIR) ; \
	done;

# remove the files installed above
uninstall:
	for app in $(APPS); do \
		rm -f $(DESTDIR)/$$app ; \
	done;

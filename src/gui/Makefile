# Makefile

CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0` -Wall -O3
LDLIBS = `pkg-config --libs gtk+-3.0` -lm

all: interface

interface: interface.o

clean:
	${RM} *.o
	${RM} *.d
	${RM} interface

# END


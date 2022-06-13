CC = gcc
CPPFLAGS= -MMD
CFLAGS= -Wall -Werror -Wextra -std=c99 -O3
LDFLAGS=
LDLIBS=

all: interface
.PHONY: interface

clean:
	(cd interface; make clean)

re: clean all

.PHONY: all clean re

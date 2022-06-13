# Simple SDL mini Makefile

CC=gcc -fsanitize=address

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image -lm -ldl


SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)
DEP= $(SRC:.c=.d)

all: main

main: $(OBJ)


clean:
	${RM} *.o
	${RM} *.d
	${RM} *.bmp
	${RM} main

# END

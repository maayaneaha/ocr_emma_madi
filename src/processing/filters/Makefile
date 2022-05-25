# Simple SDL mini Makefile

CC=gcc
CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3 
LDFLAGS=
LDLIBS= `pkg-config --libs sdl SDL_image` -lm -export-dynamic

all: processing

processing: processing.o filters.o tools.o

clean:
	${RM} *.o
	${RM} *.d
	${RM} *.bmp
	${RM} processing
	${RM} *.sh

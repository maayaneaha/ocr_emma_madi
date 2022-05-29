# Makefile

CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0 sdl` -Wall -O3 -std=c99
LDLIBS = -lm -lSDL_gfx -lSDL_ttf `pkg-config --libs gtk+-3.0 sdl SDL_image` -export-dynamic

all: interface

interface: interface.o tools.o processing.o filters.o

clean:
	${RM} *.o
	${RM} *.d
	${RM} interface
	${RM} *.bmp
# END


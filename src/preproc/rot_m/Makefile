CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= `pkg-config --cflags gtk+-3.0 sdl` -Wall -Wextra  -std=c99 -O3
LDFLAGS =
LDLIBS= `pkg-config --libs sdl gtk+-3.0` -lSDL_image -lm -lSDL_gfx

SRC = rot.c

OBJ = $(SRC:.c=.o)
DEP = $(SRC:.c=.d)

all : rot

clean :
	${RM} *.o
	${RM} *.d
	${RM} *.bmp
	${RM} main
	${RM} rot						
	${RM} rotofpicture

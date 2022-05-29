CC=gcc

CPPFLAGS= `pkg-config --cflags sdl gtk+-3.0` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3 -g
LDFLAGS=
LDLIBS= `pkg-config --libs sdl gtk+-3.0` -lSDL_image -lm -lSDL_gfx


all : construct 


construct : newreader.o pixel_operations.o construct.o

construct.o : pixel_operations.h newreader.h

clean :
	${RM} *.o
	${RM} *.d
	${RM} construct

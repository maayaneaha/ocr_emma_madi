# Makefile

CC = gcc
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra -std=c99 -O0 -g -lm
LDFLAGS =
LDLIBS =

OBJ = solvermain.o solver.o
DEP = ${OBJ:.o=.d}

all: solver

main: main.c solver.c solver.h

clean:
	${RM} ${OBJ} ${DEP} solver grid_00.result grid_01.result

solver:
	gcc -Wall -Wextra -std=c99 -O1 -o solver *.c *.h -g
-include ${DEP}

# END

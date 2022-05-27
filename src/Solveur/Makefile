# Very basic Makefile

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
SRC = resolveur.c auxiliaire_functions.c  loader.c filters.c  grid_result.c filters.c pixel_operations.c
OBJ = ${SRC:.c=.o}

resolver: pixel_operations.o loader.o grid_result.o filters.o

solver: resolveur.o
	$(CC) $(CFLAGS) resolveur.o -o solver

-include ${DEP}

clean:
	rm -f *.o
	rm -f ${DEP}
	rm -f ${PRG}
	${RM} *.result
	rm -rf *solver

# END

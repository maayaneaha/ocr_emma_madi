# Very basic Makefile

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
SRC = resolveur.c auxiliaire_functions.c
OBJ = ${SRC:.c=.o}
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

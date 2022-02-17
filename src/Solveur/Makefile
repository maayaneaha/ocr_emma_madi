# Very basic Makefile

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
SRC = resolveur.c auxiliaire_functions.c
OBJ = ${SRC:.c=.o}
solver: resolveur.o
	$(CC) $(CFLAGS) resolveur.o -o solver
# END

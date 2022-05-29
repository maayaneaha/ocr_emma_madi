CC=gcc
CPPFLAGS= -lm -MMD
CFLAGS= -Wall -Wextra -std=c99 -O2
LDFLAGS=
LDLIBS=

SRC= Network.c
DEP= ${SRC:.c=.d}
PRG= ${SRC:.c=}

all: ${PRG}

-include ${DEP}

clean:
	rm -f *.o
	rm -f ${DEP}
	rm -f ${PRG}

.PHONY: all cleanCC=gcc
	CPPFLAGS= -lm -MMD
	CFLAGS= -Wall -Wextra -std=c99 -O2
	LDFLAGS=
	LDLIBS=

SRC= Network.c
DEP= ${SRC:.c=.d}
PRG= ${SRC:.c=}

all: ${PRG}

-include ${DEP}

clean:
	rm -f *.o
	rm -f ${DEP}
	rm -f ${PRG}

.PHONY: all clean

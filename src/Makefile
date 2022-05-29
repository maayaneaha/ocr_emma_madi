CC = gcc
CPPFLAGS= -MMD
CFLAGS= -Wall -Werror -Wextra -std=c99 -O3
LDFLAGS=
LDLIBS=

all: NeuralNetwork Processing segmentation Solveur
.PHONY: NeuralNetwork Processing segmentation Solveur

NeuralNetwork:
	cd NeuralNetwork && ${MAKE}

Processing:
	cd Processing && cd rot_m && ${MAKE}
	cd Processing && cd filters && ${MAKE}

segmentation:
	cd segmentation && ${MAKE}

Solveur:
	cd Solveur && ${MAKE}

clean:
	(cd NeuralNetwork; make clean)
	(cd Processing; cd rot_m; make clean)
	(cd Processing; cd filters; make clean)
	(cd segmentation; make clean)
	(cd Solveur; make clean)

re: clean all

.PHONY: all clean re

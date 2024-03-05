OBJ = ghost.o house.o hunter.o logger.o main.o utils.o room.o
GCC = ghost.c house.c hunter.c logger.c main.c utils.c room.c
HEADER = defs.h

all: ghost
	gcc -Wall -Wextra -g -o phasmo $(OBJ) -lpthread

ghost: $(GCC) defs.h 
	gcc -c $(GCC)
	
clean:
	rm -f $(OBJ) *~ phasmo

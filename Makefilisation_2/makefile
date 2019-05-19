CFLAG=-Wall -lncursesw -lpthread
CC=gcc

all : FILE 

main.o : main.c
	gcc -c main.c -Wall

ncurses.o : ncurses.c
	gcc -c ncurses.c -lncursesw -lpthread -Wall

memory.o : memory.c 
	gcc -c memory.c -Wall -lpthread

piece.o : piece.c
	gcc -c piece.c -Wall

reseau.o : reseau.c
	gcc -c reseau.c -Wall

FILE : main.o ncurses.o piece.o memory.o reseau.o
	$(CC) -o memory main.o ncurses.o memory.o piece.o reseau.o fonctions.h variables.h $(CFLAG)

clean:
	rm -f *.o

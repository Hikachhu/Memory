CFLAGS=-Wall -lncursesw -lpthread
CC=gcc
MKDIR_P=mkdir -p
EXE=memory
OBJS=obj

all : FILE 

$(OBJS)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) -o $@ -c $< $(CFLAGS)

FILE : $(OBJS)/main.o $(OBJS)/ncurses.o $(OBJS)/piece.o $(OBJS)/memory.o $(OBJS)/reseau.o
	$(CC) -o $(EXE) $(OBJS)/main.o $(OBJS)/ncurses.o $(OBJS)/memory.o $(OBJS)/piece.o $(OBJS)/reseau.o fonctions.h variables.h $(CFLAGS)

clean:
	rm -f  $(OBJS)/*.o

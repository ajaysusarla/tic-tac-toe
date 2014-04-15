VERSION=\"0.1\"

ifeq ($(UNAME), Linux)
OSFLAGS = -DLINUX
else ifeq ($(UNAME), Darwin)
OSFLAGS = -DMACOSX
else ifeq ($(UNAME), Solaris)
OSFLAGS = -DSOLARIS
endif

CC = gcc
EXTRA_CFLAGS = -Wall -O
DEBUG = -ggdb
LIBS = -lncurses -lm
CFLAGS = $(DEBUG) $(EXTRA_CFLAGS) $(OSFLAGS)
DEFS = -DPACKAGE=$(PACKAGE) -DVERSION=$(VERSION) -DTIOCGWINSZ

all: main.o window.o game.o draw.o
	$(CC) $(CFLAGS) $(LIBS) main.o window.o game.o draw.o -o ttt

main.o: main.c ttt.h
	$(CC) $(CFLAGS) $(DEFS) -c main.c

window.o: window.c window.h
	$(CC) $(CFLAGS) $(DEFS) -c window.c

game.o: game.c game.h
	$(CC) $(CFLAGS) $(DEFS) -c game.c

draw.o: draw.c draw.h
	$(CC) $(CFLAGS) $(DEFS) -c draw.c

clean:
	rm -f ttt *.o

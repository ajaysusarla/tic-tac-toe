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

all: main.o
	$(CC) $(CFLAGS) $(LIBS) main.o -o ttt

main.o: main.c
	$(CC) $(CFLAGS) $(DEFS) -c main.c

clean:
	rm -f ttt *.o

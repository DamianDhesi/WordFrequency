CC = gcc

CFLAGS = -Wall -g

LD = gcc

LDFLAGS =

fw: main.o hashtable.o fw.h
	$(LD) $(LDFLAGS) -o fw main.o hashtable.o

all: fw

main.o: main.c fw.h
	$(CC) $(CFLAGS) -c -o main.o main.c

hashtable.o: hashtable.c fw.h
	$(CC) $(CFLAGS) -c -o hashtable.o hashtable.c

clean:
	rm *.o

# Make environment
CC = gcc
CFLAGS = -c -g -Wall

# Files
OBJECT_FILES = csapp.o shellex.o

# Productions
all: my257sh

my257sh : shellex.o csapp.o
	gcc -pthread $(OBJECT_FILES) -o my257sh

shellex.o : shellex.c csapp.h
	$(CC) $(CFLAGS) shellex.c -o shellex.o

csapp.o : csapp.c csapp.h
	$(CC) $(CFLAGS) csapp.c -o csapp.o
clean: 
	rm -v my257sh $(OBJECT_FILES)


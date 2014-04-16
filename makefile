# I am a comment, and I want to say that the variable CC will be
# the compiler to use.
CC = gcc
# Hey!, I am comment number 2. I want to say that CFLAGS will be the
# options I'll pass to the compiler.
#CFLAGS=-c -Wall
CFLAGS = -c -pedantic -w
AFLAGS = rf

# OBJECTS =

all: libpp.a pstest1.e

testing: libpp.a mytest.e

libpp.a: pp.o
	ar $(AFLAGS) libpp.a pp.o

pstest1.e:	pstest1.o libpp.a
	$(CC) pstest1.o libpp.a -o pstest1.e

mytest.e: mytest.o libpp.a
	$(CC) mytest.o libpp.a -o mytest.e
	
%.o: %.c
	$(CC) $(CFLAGS) $<

clean:
	rm *.a *o *.e 
	

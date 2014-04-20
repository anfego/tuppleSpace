# I am a comment, and I want to say that the variable CC will bethe compiler to use.
CC = gcc
MPICC = /nfshome/rbutler/public/courses/pp6430/mpich3i/bin/mpic++
# Hey!, I am comment number 2. I want to say that CFLAGS will be the
# options I'll pass to the compiler.
CFLAGS = -g -w -std=gnu++0x
AFLAGS = rf

all: libpp.a pstest1.e

testing: libpp.a mytest.e

pstest1.e: pstest1.o libpp.a
	$(MPICC) pstest1.o libpp.a -o pstest1.e

mytest.e: mytest.o libpp.a
	$(MPICC) mytest.o libpp.a -o mytest.e
	
libpp.a: pp.o lindaStuff.o lindaContact.o
	ar $(AFLAGS) libpp.a pp.o lindaStuff.o lindaContact.o

mytest.o: mytest.c
	$(MPICC) -c $(CFLAGS) mytest.c

pp.o: pp.cpp 
	$(MPICC) -c $(CFLAGS) pp.cpp 

lindaStuff.o: lindaStuff.cpp 
	$(MPICC) -c $(CFLAGS) lindaStuff.cpp 

lindaContact.o: lindaContact.cpp
	$(CC) -c $(CFLAGS) lindaContact.cpp

# pp
# %.o: %.c
# 	$(CC) -c -g -w -std=gnu++0x $<

clean:
	rm *.a *o *.e 
	

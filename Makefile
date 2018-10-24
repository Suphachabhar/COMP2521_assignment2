# Makefile for COMP1917 15s2 Week 08 Lab

CC=gcc
CFLAGS=-Wall -Werror -g
LIBS=graph.o DLList.o readData.o
BINS=pagerank invertedindex searchPagerank

all : $(BINS)

pagerank : pagerank.o readData.o DLList.o Graph.o BSTree.o
pagerank.o : pagerank.c readData.c DLList.h Graph.h BSTree.h

invertedindex : invertedindex.o readData.o DLList.o Graph.o BSTree.o
invertedindex.o : invertedindex.c readData.c DLList.h Graph.h BSTree.h

searchPagerank : searchPagerank.o set.o DLList.o
searchPagerank.o : searchPagerank.c set.h DLList.h

readData.o : readData.c readData.h
Graph.o : Graph.c Graph.h
DLList.o : DLList.c DLList.h
BSTree.o : BSTree.c BSTree.h
set.o: set.c set.h

clean :
	rm -f $(BINS) *.o core *.dSYM

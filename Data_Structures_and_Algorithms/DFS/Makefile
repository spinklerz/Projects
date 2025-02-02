#------------------------------------------------------------------------------
#  Makefile for CMPS 101 Programming Assignment 2
#
#  make                     makes FindPath
#  make GraphTest           makes GraphTest
#  make clean               removes binaries
#  make checkFind           tests FindPath for memory leaks on in7
#  make checkClient         tests GraphTest for memory leaks
#------------------------------------------------------------------------------

BASE_SOURCES   = Graph.c List.c
BASE_OBJECTS   = Graph.o List.o
HEADERS        = Graph.h List.h
COMPILE        = gcc -std=c17 -g -Wall -c
LINK           = gcc -std=c17 -Wall -o
REMOVE         = rm -f
MEMCHECK       = valgrind --leak-check=full

FindComponents : FindComponents.o $(BASE_OBJECTS)
	$(LINK) FindComponents FindComponents.o $(BASE_OBJECTS)

GraphTest : GraphTest.o $(BASE_OBJECTS)
	$(LINK) GraphTest GraphTest.o $(BASE_OBJECTS)
	

FindComponents.o : FindComponents.c $(HEADERS)
	$(COMPILE) FindComponents.c

GraphTest.o : GraphTest.c $(HEADERS)
	$(COMPILE) GraphTest.c

$(BASE_OBJECTS) : $(BASE_SOURCES) $(HEADERS)
	$(COMPILE) $*.c

clean :
	$(REMOVE) FindComponents GraphTest FindComponents.o GraphTest.o $(BASE_OBJECTS)
	rm -f *.txt *.sh

checkFind : FindPath
	$(MEMCHECK) FindComponents in7 junk7

checkClient : GraphTest
	$(MEMCHECK) GraphTest


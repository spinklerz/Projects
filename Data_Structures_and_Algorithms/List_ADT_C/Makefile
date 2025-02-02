#------------------------------------------------------------------------------
# Makefile for CSE 101 Programming Assignment 1
# Name: David Shonack
# CruzID: dshonack 
# Assignment: pa1 
# make                   makes Lex
# make ListTest       	  makes ListTest
# make clean             removes all binaries
# make checkTest      checks ListClient for memory errors
# make checkLex          checks Lex for memory errors
#------------------------------------------------------------------------------

Lex : Lex.o List.o
	gcc -std=c17 -Wall -g -o Lex Lex.o List.o

Lex.o : Lex.c List.h
	gcc -std=c17 -Wall -g -c Lex.c

ListTest: ListTest.o List.o
	gcc -std=c17 -Wall -g -o ListTest ListTest.o List.o

ListTest.o : ListTest.c List.h
	gcc -std=c17 -Wall -g -c ListTest.c

List.o : List.c List.h
	gcc -std=c17 -Wall -g -c List.c

clean :
	rm -f Lex ListTest Lex.o ListTest.o List.o

checkTest : 
	valgrind --leak-check=full ./ListTest in1 b3

checkLex :
	valgrind --leak-check=full ./Lex in1 blah3

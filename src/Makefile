# Makefile

OBJS	= bison.o lex.o main.o

CC	= g++
CFLAGS	= -g -Wall -ansi -pedantic -I../include/parser -I../include/scope

cbp:	$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o cbp -lfl

lex.o:	lex.yy.c
	$(CC) $(CFLAGS) -c lex.yy.c -o lex.o

lex.yy.c:	cbp.lex
	flex cbp.lex

bison.o:	cbp.tab.c
	$(CC) $(CFLAGS) -c cbp.tab.c -o bison.o

cbp.tab.c:	cbp.y
	bison -d -v cbp.y

main.o:	main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o main.o

lex.o yac.o main.o	: main.h
lex.o main.o		: cbp.tab.h

clean:
	rm -f *.o *~ lex.yy.c cbp.tab.c cbp.tab.h cbp.output cbp


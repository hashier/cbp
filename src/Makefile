# Makefile

OBJS	= bison.o lex.o main.o

CC	= g++
CFLAGS	= -g -Wall -ansi -pedantic

cbp:	$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o cbp -lfl

lex.o:	lex.c
	$(CC) $(CFLAGS) -c lex.c -o lex.o

lex.c:	cbp.lex
	flex cbp.lex
	cp lex.yy.c lex.c

bison.o:	bison.c
	$(CC) $(CFLAGS) -c bison.c -o bison.o

bison.c:	cbp.y
	bison -d -v cbp.y
	cp cbp.tab.c bison.c
	cmp -s cbp.tab.h tok.h || cp cbp.tab.h tok.h

main.o:	main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o main.o

lex.o yac.o main.o	: main.h
lex.o main.o		: tok.h

clean:
	rm -f *.o *~ lex.c lex.yy.c bison.c tok.h cbp.tab.c cbp.tab.h cbp.output cbp


# Makefile

OBJS	= bison.o lex.o symboltable.o scope.o main.o

CC	= g++
CFLAGS	= -g -Wall -pedantic -I../include/parser -I../include/symboltable

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
	
symboltable.o: SymbolTable.cpp
	$(CC) $(CFLAGS) -c SymbolTable.cpp -o symboltable.o

scope.o: Scope.cpp
	$(CC) $(CFLAGS) -c Scope.cpp -o scope.o

bison.o lex.o main.o	: main.h cbp.tab.h

clean:
	rm -f *.o *~ lex.yy.c cbp.tab.c cbp.tab.h cbp.output cbp


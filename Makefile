YACC=yacc
LEX=flex
CC=cc
CFLAGS=-c -g -Wall

all: 120

.c.o:
	$(CC) -c $(CFLAGS) $<

120: main.o 120gram.o 120lex.o tree.o hash.o symtable.o list.o type.o
	$(CC) -Wall -o 120 main.o 120gram.o 120lex.o tree.o hash.o symtable.o list.o type.o

120gram.c 120gram.h: 120gram.y
	$(YACC) -dt --verbose 120gram.y
	mv -f y.tab.c 120gram.c
	mv -f y.tab.h 120gram.h

main.o: main.c
	$(CC) $(CFLAGS) main.c

120lex.c: 120lex.l
	$(LEX) -t 120lex.l >120lex.c

120lex.o: 120gram.h

tree.o: tree.h

hash.o: hash.h

list.o: list.h

symtable.o: symtable.h

type.o: type.h


clean:
	rm -f 120 *.o
	rm -f 120lex.c 120gram.c 120gram.h tree.o token.o hash.o symtable.o list.o type.o

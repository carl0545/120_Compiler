YACC=yacc
LEX=flex
CC=cc
CFLAGS=-c -g

all: 120

.c.o:
	$(CC) -c $<

120: main2.o 120gram.o 120lex.o tree.o hash.o symtable.o list.o
	$(CC) -o 120 main2.o 120gram.o 120lex.o tree.o hash.o symtable.o list.o

120gram.c 120gram.h: 120gram.y
	$(YACC) -dt --verbose 120gram.y
	mv -f y.tab.c 120gram.c
	mv -f y.tab.h 120gram.h

main2.o: main2.c
	$(CC) $(CFLAGS) main2.c

120lex.c: 120lex.l
	$(LEX) -t 120lex.l >120lex.c

120lex.o: 120gram.h

tree.o: tree.h

hash.o: hash.h

list.o: list.h

symtable.o: symtable.h


clean:
	rm -f 120 *.o
	rm -f 120lex.c 120gram.c 120gram.h tree.o token.o hash.o symtable.o list.o

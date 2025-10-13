all: y.tab.o lex.yy.o
	gcc y.tab.o lex.yy.o -ly -lfl

y.tab.o: grammar.y
	yacc -d grammar.y
	gcc -c y.tab.c

lex.yy.o: lexer.l y.tab.h
	lex lexer.l
	gcc -c lex.yy.c

clean:
	rm -f *.o y.tab.c y.tab.h lex.yy.c a.out

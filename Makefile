 # -------------------------------------------------------------------
 #            Arquivo: Makefile
 # -------------------------------------------------------------------
 #              Autor: Bruno Müller Junior
 #               Data: 08/2007
 #      Atualizado em: [09/08/2020, 19h:01m]
 #
 # -------------------------------------------------------------------

$DEPURA=1

compilador: lex.yy.c y.tab.c compilador.o compiler.h utilities.h symbol_table.h
	gcc lex.yy.c compilador.tab.c utilities.c symbol_table.c compilador.o -o compilador -ll -ly -lc

lex.yy.c: compilador.l compiler.h utilities.c utilities.h
	flex compilador.l

y.tab.c: compilador.y compiler.h
	bison compilador.y -d -v

compilador.o : compiler.h compiler.c
	gcc -c compiler.c -o compilador.o

clean :
	rm -f compilador.tab.* lex.yy.c compilador.o compilador

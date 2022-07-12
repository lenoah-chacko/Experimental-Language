all: 
	gcc lex.yy.c y.tab.c -o compiler
	./compiler < $(input)
	./linker

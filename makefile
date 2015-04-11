shell: lex.yy.c y.tab.c
	gcc -g shell.c aliaslist.c preparser.c global.c lex.yy.c y.tab.c -o shell

lex.yy.c: y.tab.c shell.l
	lex shell.l

y.tab.c: shell.y
	yacc -d shell.y

clean: 
	rm -f lex.yy.c y.tab.c y.tab.h shell
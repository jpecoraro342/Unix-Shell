%{

#include <stdio.h>
#include "y.tab.h"

%}

%%

"ls"					{ return ls_command; }
"cd"				   	{ return cd_command; }
"exit"				   	{ return exit_command; }
"\""					{ return quote; }
[a-zA-Z][a-zA-Z0-9]*	{ yylval.arg = yytext; return word; }
[a-zA-Z0-9\/.-]+		{ yylval.arg = yytext; return file_name; }
;						{ return semicolon; }
\n 						{ return new_line; }
[ \t]+					/* ignore whitespace */

%%

int yywrap (void) {return 1;}
%{

#include <stdio.h>
#include "y.tab.h"

%}

%%

"ls"					{ return ls_command; }
"cd"				   	{ return cd_command; }
"setenv"				{ return setenv_command; }
"unsetenv"				{ return unsetenv_command; }
"printenv"				{ return printenv_command; }
"alias"					{ return alias_command; }
"unalias"				{ return unalias_command; }
"bye"				   	{ return exit_command; }

"\""[*? ${}_,<>|&\"@!#%'=~.:/:A-Za-z0-9]+"\""	{ yylval.arg = yytext; return quotes; }
[*?@!#%',=~.:/:A-Za-z0-9]+		{ yylval.arg = yytext; return word; }
"${"[*?@!#%,'=~.:/:A-Za-z0-9]+"}"	{ yylval.arg = yytext; return environ_var; } 
[a-zA-Z0-9_\/.-]+		{ yylval.arg = yytext; return file_name; }
;						{ return semicolon; }
\n 						{ return new_line; }
[ \t]+					/* ignore whitespace */
.						{ yylval.arg = yytext; return syntax; }


%%

int yywrap (void) {return 1;}

YY_BUFFER_STATE get_current_buffer() { return YY_CURRENT_BUFFER; }
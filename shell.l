%{

#include <stdio.h>
#include "y.tab.h"

%}

%%

"cd"				   	{ return cd_command; }
"setenv"				{ return setenv_command; }
"unsetenv"				{ return unsetenv_command; }
"printenv"				{ return printenv_command; }
"alias"					{ return alias_command; }
"unalias"				{ return unalias_command; }
"bye"				   	{ return exit_command; }

"\""[*? ${}_,<>|&\"@!#%'=~.:/:A-Za-z0-9-]+"\""	{ yylval.arg = strdup(yytext); return quotes; }
[*?@!#%'_,=~\.:/:A-Za-z0-9-]+						{ yylval.arg = strdup(yytext); return word; }

"<"						{ return read_from; }
">"						{ return write_to; }
"|"						{ return pipe_token; }
"&"						{ return ampersand; }
;						{ return semicolon; }
[ \t\n]+				/* ignore whitespace */
.						{ yylval.arg = yytext; return syntax; }


%%

int yywrap (void) {return 1;}

YY_BUFFER_STATE get_current_buffer() { return YY_CURRENT_BUFFER; }

%{

#include <stdio.h>
#include <stdlib.h>

#include "shell.h"
 
void yyerror (char *s);
int yylex();

%}

%union { char *arg; }

%type <arg> file_name;
%type <arg> word;

%start line
%token ls_command cd_command exit_command file_name word new_line

%%

line    : exit_command		{ exit(0); }
		| ls_command		{ list_files(); }
		| change_directory	{;}
		| new_line { handle_new_line(); }
		| error { syntax_error_found(); }
		| line error { syntax_error_found(); }
		| line new_line { handle_new_line(); }
		| line change_directory {;}
		| line ls_command { list_files(); }
		| line exit_command	{ exit(0); }
        ;

change_directory   	: cd_command file_name  { change_directory($2); }
					| cd_command word { change_directory($2); }   
        			;

%%

int main (void) {
	handle_new_line();
	return yyparse();
}

void yyerror (char *s) { fprintf (stderr, "%s\n", s); } 

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
//%type <arg> recursive_name;
//%type <arg> quote_input;

%start line
%token ls_command cd_command exit_command setenv_command unsetenv_command printenv_command
%token file_name word new_line quote semicolon 
%token syntax

%%

line    : command			{;}
		| line command 		{;}
		//| quote_input  		{;}
        ;

command : exit_command		{ exit(0); }
		| ls_command		{ list_files(); }
		| change_directory	{;}
		| set_environ_var	{;}
		| unset_environ_var {;}
		| print_environ_vars	{;}


		| new_line 			{ handle_new_line(); }
		| semicolon 		{;} 
		| syntax_error 		{ syntax_error_found(); printf("Syntax\n"); }
		| error 			{ syntax_error_found(); }
		;

change_directory   	: cd_command file_name  { change_directory($2); }
					| cd_command word { change_directory($2); } 
					//| cd_command quote_input { change_directory($2); }  
        			;


set_environ_var		: setenv_command word word {set_environment_variable($2, $3); }
					;

unset_environ_var	: unsetenv_command word {unset_environment_variable($2); }
					;

print_environ_vars	: printenv_command {print_environment_variables(); }
					;

syntax_error 		: syntax {;}
					;



//This is used to recognize a combination of words - useful for items where there might be a space and so the command is surrounded by quotes
//This might need to be modified
/*recursive_name : word 					{ $$ = $1; }
			   | file_name				{ $$ = $1; }
			   | recursive_name word 	{ $$ = $1; }
			   | recursive_name file_name 	{ $$ = $1; }*/

//quote_input : quote recursive_name quote { $$ = $2; }

%%

int main (void) {
	handle_new_line();
	return yyparse();
}

void yyerror (char *s) { syntax_error_found(); } 


//NOTES

//The quote stuff almost works, but not quite. I commented it out so we can just ignor it for now
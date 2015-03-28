%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shell.h"
#include "aliaslist.h"
 
void yyerror (char *s);
int yylex();

%}

%union { char *arg; }

%type <arg> file_name;
%type <arg> word;
%type <arg> recursive_name;
%type <arg> quote_input;

%start line
%token ls_command cd_command exit_command
%token setenv_command unsetenv_command printenv_command
%token alias_command unalias_command
%token file_name word new_line quote semicolon 
%token syntax

%%

/* Start */

line    : command			{;}
		| line command 		{;}
		| quote_input  		{;}
        ;

      	//Simple Commands
command : exit_command		{ exit(0); }
		| ls_command		{ list_files(); }
		| printenv_command	{ print_environment_variables(); }

		//Complex Commands
		| change_directory	{;}
		| set_environ_var		{;}
		| unset_environ_var 	{;}
		| aliasing_commands		{;}
		| unaliasing_command	{;}

		//Other Stuff
		| new_line 			{ handle_new_line(); }
		| non_command		{ syntax_error_found(); }

		//Errors
		| syntax_error 		{ syntax_error_found(); }
		| error 			{ syntax_error_found(); }
		;

non_command : file_name {;}
			| word 		{;}
			| semicolon {;}
			;

/* Complex Commands */

change_directory   	: cd_command file_name  { change_directory($2); }
					| cd_command word { change_directory($2); } 
					| cd_command quote_input { change_directory($2); } 
					| cd_command {change_directory_home(); } 
        			;

set_environ_var		: setenv_command word word  {	//Add a null terminated character at the true end of $2
													int word1len = strlen($2) - strlen($3);
													$2[word1len - 1] = '\0';
													set_environment_variable($2, $3); 
												}
					| setenv_command word file_name { //Add a null terminated character at the true end of $2
													int word1len = strlen($2) - strlen($3);
													$2[word1len - 1] = '\0';
													set_environment_variable($2, $3); 
												}
					;

unset_environ_var	: unsetenv_command word { unset_environment_variable($2); }
					;

aliasing_commands	: alias_command word word 	{ 
													int word1len = strlen($2) - strlen($3);
													$2[word1len - 1] = '\0';
													create_alias($2, $3); 
												}
					| alias_command { list_aliases(); }
					;

unaliasing_command	: unalias_command word { remove_alias($2); }
					;

/* Non-Commands */

quote_input : quote recursive_name quote { $$ = $2; $$[strlen($$) - 1] = '\0'; }

/* Recursive Helpers */

//This is used to recognize a combination of words - useful for items where there might be a space and so the command is surrounded by quotes
//This might need to be modified
recursive_name : word 						{ $$ = $1; }
			   | file_name					{ $$ = $1; }
			   | recursive_name word 		{ $$ = $1; }
			   | recursive_name file_name 	{ $$ = $1; }

/* Syntax */ 

syntax_error 		: syntax {;}
					;


%%

int main (void) {
	handle_new_line();
	return yyparse();
}

void yyerror (char *s) { syntax_error_found(); } 



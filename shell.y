%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "shell.h"
#include "aliaslist.h"
#include "global.h"

 
void yyerror (char *s);
int yylex();


%}

%union { char *cmd; char *arg; }

%type <arg> file_name;
%type <arg> word;
%type <arg> word_or_file;
//%type <arg> recursive_name;
%type <arg> quote_input;
%type <arg> quotes;

%start line
%token ls_command cd_command exit_command
%token setenv_command unsetenv_command printenv_command
%token alias_command unalias_command
%token file_name word new_line quotes semicolon 
%token read_from write_to piping ampersand
%token syntax

%%

/* Start */

line    : command			{;}
		| line command 		{;}
		//| quote_input  		{;}
        ;

      	
command : //Simple Commands
		exit_command		{ exit(0); }
		| ls_command		{ list_files(); }
		| printenv_command	{ print_environment_variables(); }

		//Built-in Commands
		| change_directory		{;}
		| set_environ_var		{;}
		| unset_environ_var 	{;}
		| aliasing_commands		{;}
		| unaliasing_command	{;}

		//Path Commands
		| path_command			{ eventCount++; builtin = 0; }

		//Other Stuff
		| new_line 			{ handle_new_line(); }
		/*| non_command		{ syntax_error_found(); }*/

		//Errors
		| syntax_error 		{ syntax_error_found(); }
		| error 			{ syntax_error_found(); }
		;

/*non_command : 
			| word 		{ check_aliases($1); }
			| quotes	{ check_aliases($1); }
			| semicolon {;}
			;*/

/* Built-in Commands */

change_directory   	: cd_command word_or_file  	{ change_directory($2); }
					| cd_command quote_input 	{ change_directory($2); } 
					| cd_command				{ change_directory_home(); } 
        			;

set_environ_var		: setenv_command word word_or_file  {	//Add a null terminated character at the true end of $2
														int word1len = strlen($2) - strlen($3);
														$2[word1len - 1] = '\0';
														set_environment_variable($2, $3); 
													}
					;

unset_environ_var	: unsetenv_command word 	{ unset_environment_variable($2); }
					;

aliasing_commands	: alias_command word word_or_file 	{ 
													int word1len = strlen($2) - strlen($3);
													$2[word1len - 1] = '\0';
													create_alias($2, $3); 
												}
					| alias_command word quote_input	{	//+1 is for quote offset
															int word1len = strlen($2) - (strlen($3) + 1);
															$2[word1len - 1] = '\0';
															create_alias($2, $3); 
														}
					| alias_command 					{ list_aliases(); }
					;

unaliasing_command	: unalias_command word 				{ remove_alias($2); }
					;

/* Path Commands */

path_command : word_or_file	{
								printf("PATH: WORD_OR_FILE: %s\n", $1);
								comtab[currcmd].comname = $1;
								//comtab[currcmd]->argptr = NIL(ARGTAB);
								comtab[currcmd].nargs = 0;
								comtab[currcmd].argptr->args[0] = $1;
							}
			 | word_or_file arguments		{
												printf("PATH: ARGUMENTS WORD_OR_FILE: %s\n", $1);
			 									comtab[currcmd].comname = $1;		
			 									comtab[currcmd].nargs = currarg;
												comtab[currcmd].argptr->args[0] = $1;
												//comtab[currcmd].argptr->args[++currarg] = $2;
											}				


/* Non-Commands */

word_or_file : word 	{ $$ = $1; }
			 | file_name 	{ $$ = $1; }
			 ;

arguments 	: word_or_file 	{ 
							printf("ARG: WORD_OR_FILE: %s\n", $1);							
							currarg = 1; /*1st arg reserved for comm.*/
							comtab[currcmd].argptr->args[currarg++] = $1; 
							}
			| arguments word_or_file	{ 
											printf("ARG: ARGUMENTS WORD_OR_FILE: %s\n", $2);
											
											comtab[currcmd].argptr->args[currarg++] = $2;  
										}
		    ;

quote_input : quotes { $$ = $1; $$[strlen($$) - 1] = '\0'; $$ = $$ + 1; }
			;

/* Recursive Helpers */

//This is used to recognize a combination of words - useful for items where there might be a space and so the command is surrounded by quotes
//This might need to be modified
/*recursive_name : word_or_file					{ $$ = $1; }
			   | recursive_name word_or_file	{ $$ = $1; }
			   ;*/

/* Syntax */ 

syntax_error 		: syntax {;}
					;


%%

int main (void) {

	//ADD HANDLE FOR CTRL+C ESCAPE
	//INIT VARIABLES/TABLES
	int i;
	for(i = 0; i<MAXCMDS; i++)
	{
		comtab[i].argptr = (ARGTAB *)malloc(sizeof(ARGTAB));
	}
	printf("Welcome to the Sperling & Pecoraro Shell!\n");
	handle_new_line();
	while (1) {	
		eventCount = 0; //USELESS RIGHT NOW
		currcmd = 0;
		currarg = 0; //First arg is reserved for the command
		builtin = 1; //Default to builtin
		/* Save the entire command line */
		char true_buffer[1024];
		preparse(true_buffer);
		parse_string(true_buffer);
		if(builtin == 0)
		{
			//CHECK IF IN TABLE CORRECTLY
			//CHECK IF IN PATH
			printf("Non built-in command entered.\n");
			i = 0;
			for (i; i<comtab[currcmd].nargs; i++)
			{
				printf("Command Name: %s | Command Arg %d: %s\n", comtab[currcmd].comname, i, comtab[currcmd].argptr->args[i]);
			}
		}	
	}

	for(i = 0; i<currcmd; i++)
	{
		free(comtab[i].argptr);
	}
	free(comtab);
}

void yyerror (char *s) { syntax_error_found(); } 



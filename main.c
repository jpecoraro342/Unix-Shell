/* main.c : main function for shell */

#include global.h
#include shell.h
#include aliaslist.h //NECESSARY?

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

int main(void)
{
	shell_init();
	while (1)
	{
		printPrompt();
		switch( command = getCommand() ) {
			CASE BYE:
				exit();
			CASE ERRORS:
				recover_from_errors();
			CASE OK:
				processCommands();
		}

	}
}


int getCommand(void)
{
	init_scanner_and_parser();
	if (yyparse())
		understand_errors();
	else
		return(OK);
}

int recover_from_errors()
{
	// Find out if error occurs in middle of command
	// That is, the command still has a “tail”
	// In this case you have to recover by “eating”
	// the rest of the command.
	// To do this: use yylex() directly.
}

int processCommands()
{
	if (builtin)
		do_it();
	// run built-in commands – no fork
	// no exec; only your code + Unix
	//system calls. 
	else
		execute_it();
	// execute general commands
	//using fork and exec

}


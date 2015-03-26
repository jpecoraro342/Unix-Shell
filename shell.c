
#include <stdio.h>
#include <unistd.h>
#include "shell.h"


/* Shell Interfacing */

/* Cleanup for when a new line has been entered - printing stuff, etc. */
void handle_new_line() {
	printf("USER:DIRECTORY $ ");
}

/* handle syntax */
void syntax_error_found() {
	//printf("Syntax Error\n");
}


/* System Functions */

/* Changes the directory to the specified path */
void list_files() {
	printf("List all the files in the current directory\n");
}

void change_directory(char * new_directory) {
	printf("Will soon change to directory: %s\n", new_directory);
}
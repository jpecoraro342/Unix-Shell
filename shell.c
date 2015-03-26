
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"


/* Shell Interfacing */

/* Cleanup for when a new line has been entered - printing stuff, etc. */
void handle_new_line() {
	char current_directory[512];
	getcwd(current_directory, sizeof(current_directory));

	char *path_extension = strrchr(current_directory, '/') + 1;

	char current_user[100];
	getlogin_r(current_user, sizeof(current_user));

	printf("%s:%s $ ", current_user, path_extension);
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
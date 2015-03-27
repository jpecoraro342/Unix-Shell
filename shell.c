//C Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Unix Libraries
#include <dirent.h> 
#include <unistd.h>
#include <errno.h>

//My Stuff
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
	DIR *d;
	struct dirent *dir;
	d = opendir(".");

	if (d) {
    	while ((dir = readdir(d)) != NULL) {
      		printf("%s\n", dir->d_name);
    	}
    	closedir(d);
  	}
  	else {
  		printf("directory empty\n");
  	}
}

void change_directory(char * new_directory) {
	int success = chdir(new_directory);
	if (success == 0) {
		char current_directory[512];
		getcwd(current_directory, sizeof(current_directory));
	}
	else {
		printf("Error Changing Directory: %d\n", errno);
	}
}

void print_error_name() {
	//TODO: Switch through all the errno and print an appropriate name/message for them
}
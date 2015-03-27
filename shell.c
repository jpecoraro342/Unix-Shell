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

extern char **environ;

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

/* Handle Syntax */
//TODO: Allow this to take a string so we can print out what the actual issue is
void syntax_error_found() {
	//printf("Syntax Error\n");
}


/* System Function Calls */

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
	if (success != 0) {
		printf("%s: %s\n", new_directory, strerror(errno));
	}
}

void set_environment_variable(char *var_name, char* new_value)
{
	printf("VARIABLE: %s \t VALUE : %s \n", var_name, new_value);
	if (setenv(var_name, new_value, 1) != 0)
	{
		printf("Cannot set variable %s to value %s : %s\n", var_name, new_value, strerror(errno));
	}
}

void unset_environment_variable(char *var_name)
{
	if (unsetenv(var_name) != 0)
	{
		printf("Cannot unset variable %s : %s\n", var_name, strerror(errno));
	}
}

void print_environment_variables(void)
{
	char ** env;
	for (env = environ; *env; ++env)
	{
    	printf("%s\n", *env);
    }
}

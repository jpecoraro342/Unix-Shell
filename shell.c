//C Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Unix Libraries
#include <dirent.h> 
#include <unistd.h>
#include <errno.h>

//Lex Stuff
typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern int yyparse();
extern YY_BUFFER_STATE yy_scan_string(char * str);
extern YY_BUFFER_STATE get_current_buffer();
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);

//My Stuff
#include "shell.h"
#include "aliaslist.h"

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

void change_directory_home(void) {
	change_directory(getenv("HOME"));
}

/* Environment Variables */

void set_environment_variable(char *var_name, char* new_value)
{
	if (setenv(var_name, new_value, 1) == 0) {
		printf("setenv - variable: %s\tvalue: %s\n", var_name, new_value);		
	}
	else {
		printf("error: setenv: could not set %s to %s : %s\n", var_name, new_value, strerror(errno));
	}
}

void unset_environment_variable(char *var_name) {
	if (unsetenv(var_name) == 0) {
		printf("unsetenv - %s\n", var_name);	
	}
	else {
		printf("error: unsetenv: could not unset %s : %s\n", var_name, strerror(errno));
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

/* Aliasing */

void list_aliases(void) {
	//Check for success
	print_aliases();
}

void create_alias(char *alias_name, char *full_command) {
	char * temp_alias = malloc(strlen(alias_name));
	char * temp_command = malloc(strlen(full_command));

	strcpy(temp_alias, alias_name);
	strcpy(temp_command, full_command);

	struct alias* a = malloc(sizeof(struct alias));
	a->alias_name = temp_alias;
	a->full_command = temp_command;

	//Check for success
	add_alias(a);

	//assume succesfull
	printf("alias - %s : %s\n", alias_name, full_command);
}


void remove_alias(char *alias_name) {
	//Check for success
	if (remove_alias_with_name(alias_name) == 0) {
		printf("alias removed : %s\n", alias_name);
	}
	else {
		printf("error: unalias: could not remove \"%s\" : alias not found\n", alias_name);
	}
}

void check_aliases(char *alias_name) {
	struct alias* tempalias = get_alias_with_name(alias_name);
	if (tempalias != NULL) {
		parse_string(tempalias->full_command);
	}
	else {
		printf("error: %s: command not found\n", alias_name);
	}
}

/* Lex/Yacc */
void parse_string(char * input) {
	YY_BUFFER_STATE cur = get_current_buffer();
	YY_BUFFER_STATE buffer = yy_scan_string(input);
    yyparse();
    yy_switch_to_buffer(cur);
    yy_delete_buffer(buffer);
}

void parse_file(char * input_file_name) {
	YY_BUFFER_STATE cur = get_current_buffer();
	YY_BUFFER_STATE buffer = yy_scan_string(input_file_name); //TODO: Change this to open up the file
    yyparse();
    yy_switch_to_buffer(cur);
    yy_delete_buffer(buffer);
}





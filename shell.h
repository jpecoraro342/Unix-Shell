#ifndef __shell__
#define __shell__

//C Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Unix Libraries
#include <dirent.h> 
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <glob.h>
#include <signal.h>
#include <pwd.h>

//Lex Stuff
typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern int yyparse();
extern void yy_switch_to_buffer (YY_BUFFER_STATE  new_buffer);
extern YY_BUFFER_STATE yy_scan_string(char * str);
extern YY_BUFFER_STATE get_current_buffer();
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);

//My Stuff
#include "aliaslist.h"
#include "global.h"
#include "preparser.h"

extern char **environ;


/* Shell Interfacing */
void shell_init(void);

/* Cleanup */
void handle_new_line();
void print_prompt();
void clear_command_table();
void free_memory();

/* Handle syntax */
void syntax_error_found();

/* System Functions */
void executeIt(void);

/* List all the files in the current directory */
void list_files();

/* Changes the directory to the specified path */
void change_directory(char * new_directory);
void change_directory_home(void);

/* Environment Variables */
void set_environment_variable(char *var_name, char* new_value);
void unset_environment_variable(char *var_name);
void print_environment_variables(void);
char * check_environment_variables(char *buffer);

/* Aliasing */
void create_alias(char *alias_name, char *full_command);
void list_aliases(void);
void remove_alias(char *alias_name);
struct alias* check_aliases(char *alias_name);

/* Lex/Yacc stuff */
void parse_string(char * input);
void parse_file(char * input_file_name);

/* IO Redirection */
void switch_output(char *file_name);
void switch_input(char *file_name);

/* Tilde Expansion */
char * tilde_expansion(char *word);
#endif
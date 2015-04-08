
/* Shell Interfacing */

/* Cleanup for when a new line has been entered - printing stuff, etc. */
void handle_new_line();

/* handle syntax */
void syntax_error_found();

/* System Functions */

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

/* Preparsing */ 
void preparse(char * true_buffer);
char* replace_environ_vars_and_aliases(char* buffer);
char* replace_aliases(char* buffer);
char* replace_environ_vars(char* buffer);


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
void set_environment_variable(char *var_name, char* new_value);
void unset_environment_variable(char *var_name);
void print_environment_variables(void);
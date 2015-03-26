
/* Shell Interfacing */

/* Cleanup for when a new line has been entered - printing stuff, etc. */
void handle_new_line();

/* handle syntax */
void syntax_error_found();


/* System Functions */

/* Changes the directory to the specified path */
void list_files();
void change_directory(char * new_directory);

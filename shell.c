#include "shell.h"

/* Shell Interfacing */

void shell_init(void) {
	printf("Welcome to the Sperling & Pecoraro Shell!\n");

	signal(SIGINT, handle_signal); //ignore ctrl c

	int i;
	for(i = 0; i<MAXCMDS; i++)
	{
		comtab[i].argptr = (ARGTAB *)malloc(sizeof(ARGTAB));
	}		
}

/* Cleanup for when a new line has been entered - printing stuff, etc. */
void handle_new_line() {
	resetVisited(); //Clears the Alias Loop Stuff
	clear_command_table(); //Clears the command table	

	currcmd = 0;
	currarg = 0; //First arg is reserved for the command
	builtin = 1; //Default to builtin
	append = 0;	 //Default to not append to <EOF> during output redirection

	/* Restore STDIN */
	if (saved_input != STDIN) {
		dup2(saved_input, STDIN);
		close(saved_input);

		input_fd = STDIN;
	}

	/* Restore STDOUT */
	if (saved_output != STDOUT) {
		dup2(saved_output, STDOUT);
		close(saved_output);

		output_fd = STDOUT;
	}

	print_prompt();
}

void print_prompt() {
	char current_directory[512];
	char *path_extension = "??";

	if (getcwd(current_directory, 512) != NULL) {
		path_extension = strrchr(current_directory, '/') + 1;
	}

	char current_user[100];
	if (getlogin_r(current_user, 100) != 0) {
		current_user[0] = '?';
		current_user[1] = '?';
		current_user[2] = '\0';
	}

	printf("%s:%s $ ", current_user, path_extension);
}

void clear_command_table() {
	int i, j;
	for(i=0; i<=currcmd; i++)
	{
		for(j=0; j<=comtab[i].nargs; j++)
		{
			comtab[i].argptr->args[j] = NULL;
		}
		comtab[i].comname = NULL;
		comtab[i].nargs = 0;
	}
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
	else {
		char *var_name = "PWD";
		char current_directory[512]; 
		getcwd(current_directory, sizeof(current_directory));
		setenv(var_name, current_directory, 1);
	}
}

void change_directory_home(void) {
	change_directory(getenv("HOME"));
}

/* Environment Variables */

void set_environment_variable(char *var_name, char* new_value)
{
	if (setenv(var_name, new_value, 1) == 0) {
		printf("setenv - variable: \"%s\"\tvalue: \"%s\"\n", var_name, new_value);		
	}
	else {
		printf("error: setenv: could not set \"%s\" to \"%s\" : %s\n", var_name, new_value, strerror(errno));
	}
}

void unset_environment_variable(char *var_name) {
	if (unsetenv(var_name) == 0) {
		printf("unsetenv - \"%s\"\n", var_name);	
	}
	else {
		printf("error: unsetenv: could not unset \"%s\" : %s\n", var_name, strerror(errno));
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
	char * temp_alias = malloc(strlen(alias_name) + 1);
	char * temp_command = malloc(strlen(full_command) + 1);

	strcpy(temp_alias, alias_name);
	strcpy(temp_command, full_command);

	struct alias* a = malloc(sizeof(struct alias));
	a->alias_name = temp_alias;
	a->full_command = temp_command;
	a->visited = false;

	//Remove old alias with name if it exists
	remove_alias_with_name(alias_name);

	//Check for success
	add_alias(a);

	//assume succesfull
	printf("alias - \"%s\" : \"%s\"\n", alias_name, full_command);
}


void remove_alias(char *alias_name) {
	//Check for success
	if (remove_alias_with_name(alias_name) == 0) {
		printf("alias removed : \"%s\"\n", alias_name);
	}
	else {
		printf("error: unalias: could not remove \"%s\" : alias not found\n", alias_name);
	}
}

struct alias* check_aliases(char *alias_name) {
	struct alias* tempalias = get_alias_with_name(alias_name);
	/*if (tempalias != NULL) {
		printf("alias found - \"%s\" : \"%s\"\n", alias_name, tempalias->full_command);
	}
	else {
		printf("alias not found - \"%s\" \n", alias_name);
	}*/
	return tempalias;
}

/* Lex/Yacc */

void parse_string(char * input) {
	//puts(input);
	//YY_BUFFER_STATE cur = get_current_buffer();
	YY_BUFFER_STATE buffer = yy_scan_string(input);
    yyparse();
   	//yy_switch_to_buffer(cur);
   	//yy_delete_buffer(buffer); //MAJOR ISSUE!!!
}

void parse_file(char * input_file_name) {
	/*
	FILE *in = fopen(input_file_name, "r");
	if (in == NULL) {
		printf("error: invalid file: %s\n", input_file_name);
		return;
	}

    fseek(in, 0, SEEK_END);
    unsigned long len = (unsigned long)ftell(in);

	YY_BUFFER_STATE cur = get_current_buffer();
	YY_BUFFER_STATE buffer = yy_scan_buffer(in, len);
    yyparse();
    yy_switch_to_buffer(cur);
    yy_delete_buffer(buffer);
    */
}

/* IO Redirection */

void switch_output(char *file_name) {
	int file_descriptor;

	file_descriptor = open(file_name, O_WRONLY | O_CREAT, S_IREAD | S_IWRITE );
	if (file_descriptor == -1) {
		printf("error: unable to open file: %s\n", strerror(errno));
		return;
	}

	output_fd = file_descriptor;

	saved_output = dup(STDOUT);
}

void switch_input(char *file_name) {
	int file_descriptor;

	file_descriptor = open(file_name, O_RDONLY);
	if (file_descriptor == -1) {
		printf("error: unable to open file: %s\n", strerror(errno));
		return;
	}

	input_fd = file_descriptor;

	saved_input = dup(STDIN);
}

//Process Handling

void executeIt(void)
{
	pid_t process = fork();

	if (process > 0)			/* parent */
		wait ((int *) 0);		/* null pointer - return value not saved */
	else if (process == 0)		/* child */
	{	/* execute program */
		execvp (comtab[currcmd].comname, comtab[currcmd].argptr->args);

		/* some problem if exec returns */
		printf("error: %s: %s\n", comtab[currcmd].comname, strerror(errno));
		exit (1);
	}
	else if ( process == -1)     /* can't create a new process */
	{
		printf ("error: can't fork process: %s", strerror(errno));
		exit (2);
	}
}

void handle_signal(int signo) {
	if (signo == SIGINT) {
		printf("to exit please enter bye");
		handle_new_line();
	}
}

//Clean-up

void free_memory(void)
{
	int i;
	for(i = 0; i<MAXCMDS; i++)
	{
		free(comtab[i].argptr);
	}
}

#include "preparser.h"

/* Pre Parsing */

char* replace_environ_vars_and_aliases(char* buffer) {
	//printf("Orignal Input: \"%s\"\n", buffer);
	if (strstr(buffer, "alias") == buffer) {
		//printf("Don't Replace Variables\n");
		return buffer;
	}

	char* alias_buffer = replace_aliases(buffer);
	if (alias_buffer == NULL) {
		return "";
	}
	//printf("Post Alias Input: \"%s\"\n", alias_buffer);
	char* environ_buffer = replace_environ_vars(alias_buffer);
	char* wildcard_buffer = wildcard_expansion(environ_buffer);

	//printf("Post Pre-Parsing Input: \"%s\"\n", environ_buffer);
	return wildcard_buffer;
}

char* replace_aliases(char* buffer) {
	//printf("replace aliases in \"%s\"\n", buffer);
	char *buffer_copy = malloc(strlen(buffer) + 1);
	strcpy(buffer_copy, buffer);
	char *first_word = malloc(strlen(buffer) + 1);

	if (strchr(buffer_copy, ' ') != NULL) {
		first_word = strtok(buffer_copy, " \"");
	}
	else {
		first_word = strtok(buffer_copy, "\'\n\'");
	}
	//printf("First Word: %s\n", first_word);
	
	if (first_word != NULL) {
		struct alias* alias = check_aliases(first_word);

		if (alias == NULL) {
			//printf("No Alias Found for: \"%s\"\n", first_word);
			return buffer;
		}
		else if (alias->visited) {
			printf("error: alias loop: \"%s\" : \"%s\"\n", first_word, alias->full_command);
			return NULL;
		}
		else {
			//printf("Alias Found: \"%s\" : \"%s\"\n", first_word, alias->full_command);
			alias->visited = true;

			//Replace the String
			int original_length = strlen(first_word);
			int replace_length = strlen(alias->full_command);
			int final_new_length = strlen(buffer) - original_length + replace_length;

			//copy of replacement string - not sure if we need to do this
			char *full_command = malloc(replace_length + 1);
			strcpy(full_command, alias->full_command);

			//create a new string
			//should malloc to final_new_length + 1, but need extra space for environ vars
			char *replacement_buffer = malloc(1024);
			strcat(replacement_buffer, full_command);
			strcat(replacement_buffer, buffer+original_length);

			//printf("Replaced Alias: \"%s\" : \"%s\"\n", first_word, alias->full_command);
			//printf("New Command: %s\n", replacement_buffer);

			return replace_aliases(replacement_buffer);
		}
	}

	free(buffer_copy);

	return buffer;
}

char* replace_environ_vars(char* buffer) {
	char* temp_buffer = check_environment_variables(buffer);
	if(temp_buffer != NULL)
	{
	    //puts(temp_buffer);
	    while((temp_buffer != NULL) && (strcmp(temp_buffer,buffer) != 0))
	    {
			strcpy(buffer,temp_buffer);
			free(temp_buffer);
			temp_buffer = check_environment_variables(buffer);
	    }
	}
	char *returned_buffer = malloc(1024);
	memcpy(returned_buffer, buffer, 1024);

	//returned_buffer = realloc(returned_buffer, strlen(returned_buffer) + 1);
	return returned_buffer;
}

char* check_environment_variables(char *buffer)
{
	char *start_pointer;
	char *end_pointer;
	char environ_buffer[1024];

	char *return_buffer;
	char *return_buffer_reference;

	
	if((start_pointer = strstr(buffer, "${")) != NULL)
	{
		if((end_pointer = strchr(start_pointer+2, '}')) != NULL)
		{

			int size = end_pointer - start_pointer-2;
			//initialize environ_buffer to 0 before strncopy
			memset(environ_buffer, '\0', 1024);
			strncpy(environ_buffer, start_pointer+2, size);
			//puts(environ_buffer);

			if(getenv(environ_buffer) != NULL)
			{
				//printf("%s\n", getenv(environ_buffer));
				return_buffer = calloc(1024,sizeof(char));
				int before_var_size = start_pointer - buffer + 1;
				strncpy(return_buffer, buffer, before_var_size-1);
				strcat(return_buffer, getenv(environ_buffer));

				return_buffer_reference = &return_buffer[strlen(return_buffer)-1];
				strncpy(return_buffer_reference+1, end_pointer+1, strlen(end_pointer)-1);

				//puts(return_buffer);

				//return_buffer = realloc(return_buffer, strlen(return_buffer) + 1);
				return return_buffer;
			}
			else
			{
				printf("Environment variable \"%s\" not found.\n", environ_buffer);
				return NULL;			
			}
			
		}
	}
	return NULL;
}

char * wildcard_expansion(char * buffer) {
	//make a copy so we can tokenize it
	char *buffer_copy = malloc(strlen(buffer) + 1);
	strcpy(buffer_copy, buffer);

	//if there are no spaces, we can't expand it.. I think?
	if (strchr(buffer_copy, ' ') != NULL) {
		//remove newline
		char *newline = strchr(buffer_copy, '\n');
		if (newline != NULL) {
			int loc = (int) (newline - buffer_copy);
			buffer_copy[loc] = '\0';
		}

		char* word = strtok(buffer_copy, " \"");

		//Look through each word until we find a matching * or ?
		while (word != NULL && strchr(word, '*') == NULL && strchr(word, '?') == NULL) {
			word = strtok(NULL, " \"");
		}
		
		//If we couldnt find any patterns, return
		if (word == NULL) {
			free(buffer_copy);
			return buffer;
		}
		else {
			//The pattern location in the actual buffer
			char *pattern = strstr(buffer, word);
			int patternlength = strlen(word);
			int patternlocation = (int) (pattern - buffer);
			
			//Glob stuff
			glob_t globbuf;
			//printf("pattern \"%s\"\n", word);
			if (glob(word, 0, NULL, &globbuf) != 0) {
				//printf("no matching files or error, return original silently\n");
				free(buffer_copy);
				return buffer;
			}

			char *globstringbuff = malloc(4096);
			globstringbuff[0] = '\0';
			
			int i;
			for (i = 0; i < globbuf.gl_pathc; i++) {
				strcat(globstringbuff, globbuf.gl_pathv[i]);
				if (i != globbuf.gl_pathc - 1) {
					strcat(globstringbuff, " ");
				}
			}
			
			char *new_buffer = malloc(4096);
			new_buffer[0] = '\0';
			strncpy(new_buffer, buffer, patternlocation);
			new_buffer[patternlocation] = '\0';
			
			strcat(new_buffer, globstringbuff);
			strcat(new_buffer, pattern + patternlength);
			
			//Shrink new_buffer, free allocated memory
			new_buffer = realloc(new_buffer, strlen(new_buffer) + 1);
			free(globstringbuff);
			free(buffer_copy);
			globfree(&globbuf);
			
			return wildcard_expansion(new_buffer);
		}
		
	}
	return buffer;
}

/* Reads the entire command line from the terminal, parses environment variables, and aliases. */
void preparse(char * true_buffer) {
	char buffer[1024];
	fgets(buffer, 1024, stdin);

	if (feof(stdin)) {
		strcpy(true_buffer, "bye");
		return;
	}

	char * return_buffer = replace_environ_vars_and_aliases(buffer);
	strcpy(true_buffer, return_buffer);
}

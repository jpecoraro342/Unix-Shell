#ifndef __preparser__
#define __preparser__

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

//My Stuff
#include "shell.h"
#include "aliaslist.h"
#include "global.h"

/* Preparsing */ 
void preparse(char * true_buffer);
char* replace_environ_vars_and_aliases(char* buffer);
char* replace_aliases(char* buffer);
char* replace_environ_vars(char* buffer);
char* wildcard_expansion(char* buffer);

#endif
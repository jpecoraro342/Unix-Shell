#ifndef __global__
#define __global__

/* global.h : global data strcutures and constants for shell */

#include <stdbool.h>

#define MAXCMDS 50
#define MAXARGS 300
#define MAXALIAS 100
#define MAXPATH 50 //NECESSARY?

#define OK 0
#define SYSERR 1
#define SYSCALLERR -1

#define THE_ONLY_ONE 1
#define FIRST 2
#define LAST 3

#define PATH_EXECUTABLE builtin == 0

#define STDIN  0
#define STDOUT 1
#define STDERR 2

//File IO
extern int saved_input;
extern int saved_output;

extern int input_fd;
extern int output_fd;

extern bool execute_in_background;

/* command line arguments structure */
typedef struct comargs {
	char *args[MAXARGS];
} ARGTAB;

/* command line structure */
typedef struct com {
	char *comname;
	//int remote;
	int infd;
	int outfd;
	int nargs;
	ARGTAB *argptr;
} COMMAND;


/* Externals */
extern COMMAND comtab[MAXCMDS];
extern int eventCount;
extern int builtin;
extern int currcmd;
extern int currarg;
extern int append;


#define NIL(x) (x *)0

#endif
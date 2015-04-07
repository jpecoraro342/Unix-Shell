/* global.h : global data strcutures and constants for shell */

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

/* command line arguments structure */
typedef struct comargs {
	char *args[MAXARGS};
} ARGTAB

/* command line structure */
typedef struct com {
	char *comname;
	int remote;
	int infd;
	int outfd;
	int nargs;
	ARGTAB *atptr;
} COMMAND;
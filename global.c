/* global.c */

#include "global.h"

int builtin;
int eventCount;
int currcmd;
int currarg;
int append;
COMMAND comtab[MAXCMDS];

//File IO
int saved_output = STDOUT;
int saved_input = STDIN;

int input_fd = STDIN;
int output_fd = STDOUT;
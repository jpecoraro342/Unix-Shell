/* pipes.c : Functions to handle piping between commands */

#include <stdio.h>
#include <unistd.h>
//#include <sys/types.h>

#include "shell.h"
#include "global.h"

int fork_pipes (void)
{
	int i;
	pid_t pid;
	int in, fd[2];

	in = 0;

	for(i = 0; i< currcmd; i++;)
	{
		pipe(fd);

		spawn_process(int in, fd[1], i);

		close(fd[1]); //Close write end of pipe.

		in = fd[0]; //Save the read end of the pipe.
	}
}


int spawn_process (int in, int out, int currcmd)
{

}
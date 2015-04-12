/* pipes.c : Functions to handle piping between commands */

#include "pipes.h"

void fork_pipes (void)
{
	int i;
	pid_t pid;
	int in, fd[2];

	in = input_fd; //For Input redirection. Defaults to STDIN.
	printf("WE MADE IT STEP 1\n");
	for(i = 0; i < currcmd; i++)
	{
		pipe(fd);
		printf("WE MADE IT STEP 2\n");
		// fd[1] is write-end of the pipe, and we carry 'in' from prevous iteration.
		spawn_process(in, fd[1], i);

		printf("WE MADE IT STEP 3\n");
		close(fd[1]); //Close write end of pipe; child will write here.

		in = fd[0]; //Save the read end of the pipe.
		printf("WE MADE IT STEP 4\n");
	}
	printf("WE MADE IT OUT OF THE LOOP\n");
	//Handle input for the final command.
	if (in != STDIN)
	{
		dup2 (in, STDIN);
		printf("WE MADE IT TO THE FINAL COMMAND PART 1\n");
	}

	//Output redirection for the final command. Defaults to STDOUT.
	if(output_fd == SYSCALLERR)
		printf("Error 2: Cannot execute pipe: invalid output file\n");
	else
		dup2 (STDOUT, output_fd);
	printf("WE MADE IT TO THE FINAL COMMAND PART 2\n");

	pid = fork();

	if (pid > 0)			/* parent */
		wait ((int *) 0);		/* null pointer - return value not saved */
	else if (pid == 0)
	{	
		printf("WE MADE IT TO THE FINAL COMMAND PART 3\n");
		//Execute the final command in the pipeline
		execvp (comtab[currcmd].comname, (char * const *)comtab[currcmd].argptr->args);

		printf("Error 3: %s: %s\n", comtab[currcmd].comname, strerror(errno));
		exit(1);
	}
	else
	{
		printf("Error: Cannot fork process.\n");
	}

}


int spawn_process (int in, int out, int current_cmd)
{
	printf("WE MADE IT TO THE SPAWN\n");
	pid_t pid = fork();
	//ADD ERROR CHECKING
	printf("WE MADE IT PAST THE SPAWN FORK: pid = %d\n", pid);
	if (pid > 0)			/* parent */
	{
		printf("WE'RE WAITING.\n");
		wait ((int *) 0);		/* null pointer - return value not saved */
	}
	else if (pid == 0)
	{
		printf("WE'RE NOT WAITING ANYMORE\n");

		if (in != STDIN)
		{
			printf("Duplicate IN\n");
			dup2(in, STDIN);
			close(in);
		}

		if(out != STDOUT)
		{
			printf("Duplicate OUT\n");
			dup2(out, STDOUT);
			close(out);
		}

		printf("ABOUT TO LEAVE THE SPAWN: \n");
		execvp(comtab[current_cmd].comname, (char * const *)comtab[current_cmd].argptr->args);	
		printf("Error 1: %s: %s\n", comtab[current_cmd].comname, strerror(errno));
		exit(1);
	}
	else
	{
		printf("Error: Cannot fork process.\n");
	}
	
	return pid;
}
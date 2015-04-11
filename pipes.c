/* pipes.c : Functions to handle piping between commands */


int fork_pipes (void)
{
	int i;
	pid_t pid;
	int in, fd[2];

	in = input_fd; //For Input redirection. Defaults to STDIN.

	for(i = 0; i < currcmd; i++)
	{
		pipe(fd);

		// fd[1] is write-end of the pipe, and we carry 'in' from prevous iteration.
		if (spawn_process(int in, fd[1], i) != 0)
		{
			printf("Error: Cannot execute pipe: %s", strerror(errno));
			return;
		}

		close(fd[1]); //Close write end of pipe; child will write here.

		in = fd[0]; //Save the read end of the pipe.
	}

	//Handle input for the final command.
	if (in != STDIN)
	{
		dup2 (in, STDIN);
	}

	//Output redirection for the final command. Defaults to STDOUT.
	if(output_fd == SYSCALLERR)
		//INVALID FILE DESCRIPTOR. SKIP DUP
	else
		dup2 (STDOUT, output_fd);

	//Executwe the final command in the pipeline
	return execvp (comtab[currcmd].comname, comtab[currcmd].argptr->args);
}


int spawn_process (int in, int out, int current_cmd)
{
	pid_t pid = fork();
	//ADD ERROR CHECKING

	if (pid == 0)
	{
		if (in != STDIN)
		{
			dup2(in, STDIN);
			close(in);
		}

		if(out != STDOUT)
		{
			dup2(out, STDOUT);
			close(out);
		}

		return execvp (comtab[current_cmd].comname, (char * const *)comtab[current_cmd].argptr->args);	
	}

	return pid;
}
/* pipes.c : Functions to handle piping between commands */

#include "pipes.h"

void fork_pipes (void)
{
	int i;
	pid_t pid;
	int fd [currcmd][2];

	for (i = 0; i < currcmd; i++) {
		pipe(fd[i]);
	}

	//printf("created %d pipes\n", currcmd);

	for (i = 0; i <= currcmd; i++) {
		pid = fork();

		int in_fd = fd[i][0];
		int out_fd = fd[i][1];

		if(pid < 0) {
			printf("Error: Could not fork process\n");
			return;
		}
		else if (pid == 0) {
			//printf("child process running: %d\n", i);

			if (i != 0) {
				int last_in = fd[i-1][0];
				dup2(last_in, STDIN);
			}
			else {
				dup2(in_fd, input_fd);
			}

			if (i != currcmd) {
				dup2(out_fd, STDOUT);
			}
			else {
				dup2(fd[i-1][1], output_fd);
			}

			close(in_fd);

			execvp (comtab[i].comname, (char * const *)comtab[i].argptr->args);
			printf("Error: %s: %s\n", comtab[i].comname, strerror(errno));
			exit(1);
		}
		else {
			//wait((int *) 0);
			close(out_fd);
		}
 	}

 	 for (i = 0; i <= currcmd; i ++) {
 	 	wait((int *) 0);
 	}

 	// for (i = 0; i < currcmd; i ++) {
 	// 	close(fd[i][0]);
 	// 	close(fd[i][1]);
 	// }
}


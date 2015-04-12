/* pipes.h */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

#include "shell.h"
#include "global.h"

void fork_pipes(void);
int spawn_process(int in, int out, int current_cmd);
Sperling & Pecoraro Unix Shell
=========

####Features NOT Implemented
- Standard Error Redirection
- I/O Redirection for Built-in Commands (we were told this was not necessary)
- Piping for Built-in Commands (we were also told this was not necessary)
- Executing Commands in the Background
- Printing Line Number of Errors
- Tilde Expansion *Extra Credit* (Almost working, can show progress on git)
- File Name Completion *Extra Credit*

#####PATH COMMANDS DO NOT RUN WHEN REDIRECTING STANDARD INPUT FROM A FILE.
Please try to run commands in the shell itself on the command line. Sorry for the inconvenience.

####Features Implemented
- Built-in Commands
- Path (all other) Commands
- Pipes
- I/O Redirection to/from Files (with one input problem mentioned above)
- Environment Variables / Environment Variables Expansion
- Aliasing
- Wildcard Matching

###Compile Instructions
1. Run "make clean".
2. Run "make".
3. Run "./shell".

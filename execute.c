/* execute.c - code used by small shell to execute commands */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

int execute(char *argv[])
/*
 * purpose: run a program passing it arguments
 * returns: status returned via wait, or -1 on error
 *  errors: -1 on fork() or wait() errors
 */
{
  int	child_info = -1;
  int background, pid;

  //Get length of arglist
  int args = 0;
  while(argv[args] != NULL){
    args++;
  }

  //Do nothing if there are no arguments
  if(args == 0)
    return 0;

  //Checks if last argument is background flag
  if(background = strcmp("&", argv[args - 1]) == 0){
      argv[args] = NULL;
  }

  //Fork process
  if ((pid = fork())  == -1)
    perror("fork");

  //Child process executes command
  else if (pid == 0){
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    execvp(argv[0], argv);
    perror("cannot execute command");
    exit(0);
  }

  //Return child PID if running in background
  else if(background){
    printf("Started process %d.\n", pid);
    return pid;
  }

  //Wait for child to finish if running in foreground
  else if (waitpid(pid, &child_info, WUNTRACED) == -1 ){
      perror("wait");
  }

  return 0;
}

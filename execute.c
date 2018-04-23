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
  int	pid;
  int	child_info = -1;
  int background = -1;

   // Gets length of arglist
   int args = 0;
   while(argv[args] != NULL){
      args++;
   }

   if(background = strcmp("&", argv[args - 1]) == 0){
      argv[args] = NULL;
   }

  if ( argv[0] == NULL )		/* nothing succeeds	*/
    return 0;

  if ( (pid = fork())  == -1 )
    perror("fork");
  else if ( pid == 0 ){
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    execvp(argv[0], argv);
    perror("cannot execute command");
    exit(0);
  }else{
    if(background){
      printf("Started process %d.\n", pid);
      return pid;
    }
    else if (waitpid(pid, &child_info, WUNTRACED) == -1 ){
      perror("wait");
    }
  }
  return 0;
}

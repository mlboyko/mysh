/**  COS 350 Program 5 - mysh.c
 **   Nick Johnston
 **		Justin O'Malley
 **   Makayla Boyko 
 **/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include "smsh.h"

#define	DFL_PROMPT	"The Best Shell > "

void setup();

int main()
{
  struct passwd *pw = getpwuid(getuid());
  const char *homedir = pw->pw_dir;

  char	*cmdline, *prompt, **arglist;

  prompt = DFL_PROMPT;
  setup();

  //Get the next line
  while ((cmdline = next_cmd(prompt, stdin)) != NULL){

    //Split the line into tokens (arglist)
    if ((arglist = splitline(cmdline)) != NULL){

      //Do nothing, avoid segmentation fault from strcmp on null string
      if(arglist[0] == NULL){
      }

      //Exit
      else if (strcmp("exit", arglist[0]) == 0){
        if(arglist[1] != NULL)
          return atoi(arglist[1]);

        return 0;
      }

      //Change Directory
      else if(strcmp("cd", arglist[0]) == 0){

        if (arglist[1] != NULL)
          chdir(arglist[1]);
        else
          chdir(homedir);
      }

      //Execute a different command
      else if(arglist[0] != NULL){
        int result = execute(arglist);
        freelist(arglist);
      }
    }


    int status, pid;

    //Notify user of all finished child processes
    while((pid = waitpid(0, &status, WNOHANG)) > 0)
      printf("Background process %d complete.\n", pid);

    free(cmdline);
  }

  return 0;
}

void setup()
/*
 * purpose: initialize shell
 * returns: nothing. calls fatal() if trouble
 */
{
  signal(SIGINT,  SIG_IGN);
  signal(SIGQUIT, SIG_IGN);
}

void fatal(char *s1, char *s2, int n)
{
  fprintf(stderr,"Error: %s,%s\n", s1, s2);
  exit(n);
}


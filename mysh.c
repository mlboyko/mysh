/**  smsh1.c  small-shell version 1
 **		first really useful version after prompting shell
 **		this one parses the command line into strings
 **		uses fork, exec, wait, and ignores signals
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
#define MAX_BG_PROCESSES 100

int main()
{
  struct passwd *pw = getpwuid(getuid());
  const char *homedir = pw->pw_dir;

  char	*cmdline, *prompt, **arglist;
  int bgPIDs [MAX_BG_PROCESSES];
  int	result;
  int background;
  int bgPIDCount = 0;
  void	setup();

  prompt = DFL_PROMPT ;
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
        result = execute(arglist);
        freelist(arglist);

        //If background process, store PID
        if(result > 0){
          bgPIDs[bgPIDCount] = result;
          bgPIDCount++;
        }
      }
    }


    int i = 0, status;

    //Iterate through PIDs of every background process
    while(i < bgPIDCount){

      //If process has returned
      if (waitpid(bgPIDs[i], &status, WNOHANG) > 0){

        //Notify user
        printf("Background process %d complete.\n", bgPIDs[i]);
        printf("There are this many bgProcesses: %d\n", bgPIDCount - 1);

        //Shift array of background PIDs to the left to fill gap
        for (int k = i; k < bgPIDCount - 2; k--){
          bgPIDs[k] = bgPIDs[k + 1];
          printf("Success!\n");
        }

        bgPIDCount--;
  
      }else{
        i++;
      }
    }
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


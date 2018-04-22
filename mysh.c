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
#include <pwd.h>
#include "smsh.h"

#define	DFL_PROMPT	"The Best Shell > "

int main()
{
  struct passwd *pw = getpwuid(getuid());
  const char *homedir = pw->pw_dir;

  char	*cmdline, *prompt, **arglist;
  int	result;
  void	setup();

  prompt = DFL_PROMPT ;
  setup();

  while ( (cmdline = next_cmd(prompt, stdin)) != NULL ){
    if ( (arglist = splitline(cmdline)) != NULL  ){
      if( strcmp("exit", arglist[0]) == 0){
        if( arglist[1] != NULL) {
            return atoi(arglist[1]);
        }
        return 0;
      }
      if( strcmp("cd", arglist[0]) == 0 ){
        if (arglist[1] != NULL){
            chdir(arglist[1]);
        }else{
            chdir(homedir);
        }
      }else if ( strcmp("&", arglist[2]) == 0){
        printf("should background a proccess here");
      }else{
        result = execute(arglist);
        freelist(arglist);
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


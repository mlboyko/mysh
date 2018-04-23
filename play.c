/* execute.c - code used by small shell to execute commands */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
  for(int i = 0; i < 1; i++){
    printf("hello %d\n", i);
    sleep(10);
    
  }

  return 0;
}

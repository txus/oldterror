#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

void die(const char *message)
{
  if(errno) {
    perror(message);
  } else {
    printf("ERROR: %s\n", message);
  }

  exit(1);
}

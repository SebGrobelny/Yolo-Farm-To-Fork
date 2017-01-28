#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "hstack.h"


#define MAX_SIZE 10


/* Initialize a hstack */
void init_hstack(hstack *hs)
{
  int i;
  for (i = 0; i < MAX_SIZE; i++) {
    hs->saved[i] = NULL;
  }

  hs->top = -1;

}

void free_hstack(hstack *hs)
{
  int i;
  for (i = 0; i < MAX_SIZE; i++) {
    free(hs->saved[i]);
  }
  free(hs);

}

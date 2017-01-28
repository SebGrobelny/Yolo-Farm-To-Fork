#ifndef HSTACK_H
#define HSTACK_H

#define MAX_SIZE 10

typedef struct hstack {
  char *saved[MAX_SIZE]; // Array of pointers to the saved commands
  int top;
} hstack;

void init_hstack(hstack *);
void free_hstack(hstack *);



#endif /*HSTACK_H */

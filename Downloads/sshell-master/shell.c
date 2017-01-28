#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "arrow.h"    /* Definitions of reading input w/ termios */
#include "parser.h"   /* Definitions of our parse struct matrices */
#include "hstack.h"


#define PATH_MAX 4096 //max number of bits held by an M4K memory block
#define BACKGROUND_MAX 16 //max number background proccesses you can have at once

//A struct that holds a background job's pid and command name, used for completed printing
typedef struct backgroundJob{
    char *jobName;     // contains what was originally entered int he command line
    pid_t jobPid;         // the pid of the job
} backgroundJob;

//prototypes
int catchBuiltInCommand(parseMatrix *);
int s_cd(char **);
int s_pwd(char **);
void executeCommand(parseMatrix *);
void init_backgroundJobs(backgroundJob []);
void free_backgroundJobs(backgroundJob []);
void catchRedirection(parseMatrix *);
void catchPipe(parseMatrix *);

int main (int argc, char **argv)
{


  int i;                    /*used in for loops*/
  char * cmdLine;            /*Char buffer storing user input */
  parseMatrix *pmatrix;     /*matrix stores all the information returned by parser.*/
  commandMatrix *cmatrix;   /*com stores command name and Arg list for one command.*/
  int isBuiltin;            /*boolean: stores if there was a builtin function or not*/
  pid_t pid;              /*  process id number */
  int status;

  backgroundJob bJobs[BACKGROUND_MAX]; /*An array of background jobs*/


  //bJobs = malloc((BACKGROUND_MAX) * sizeof(backgroundJob)); // Declare and allocate memoery for the result
  init_backgroundJobs(bJobs);

  hstack *history = malloc(sizeof(hstack));
  init_hstack(history);


  while(1){
    /* Check for zombies
    pid = waitpid(-1, &status, WNOHANG);
    if (pid > 0)
        printf("waitpid reaped child pid %d\n", pid); */

    /*insert your code to print prompt here*/
    /*printf("8==D ");*/

    cmdLine = read_input(history);

    //printf("recorded command: %s\n", history->commandList[history->h_pos]);


    //printf("h_index: %d\n",history->h_index);


    /*instance of Ctrl-D*/
    if (cmdLine == NULL) {
      if (pid == 0){
        printf("Exiting with child\n");
      }
      printf("exiting via ctrl-d\n");
      free_hstack(history);
      free_backgroundJobs(bJobs);
      printf("\nBye...\n");
      return 0;

    }

    /*insert your code about history and !x !-x here*/

    /*calls the parser*/
    pmatrix = parse(cmdLine);
    if (pmatrix == NULL) {
      free(cmdLine);
      continue;
    }
    /*prints the info struct*/
    //print_matrix(info);

    /*com contains the info. of the command before the first "|"*/
    cmatrix=&pmatrix->commandArr[0];
    if ((cmatrix == NULL)  || (cmatrix->command == NULL)) {
      free_parseMatrix(pmatrix);
      free(cmdLine);
      continue;
    }

    /* checks for builtin AND executes that command*/
    isBuiltin = catchBuiltInCommand(pmatrix);

    /* Exit check */
    if (isBuiltin < 0) {
      printf("Bye...\n");
      free_hstack(history);
      free_backgroundJobs(bJobs);
      return 0;
    }

    

    /* Launch a new Process */
    if (isBuiltin == 0) {

       pid = fork();

       /* Catch fork failure */
       if (pid < 0) {
            printf("Error: fork failed.");
            exit(1);

       }
       if (pid == 0) {  // child
            //printf("\nI am child %d, group %d. \n", getpid(), getpgid(getpid()));

            //printf("Checking for pipes\n");
            if(pmatrix->num_pipes > 0) {
              //printf("Pipe caught\n");
              catchPipe(pmatrix);
              //waitpid(pid, &status, 0);
            }
            catchRedirection(pmatrix);

            /*call to pipe*/
          
            if(pmatrix->num_pipes == 0) executeCommand(pmatrix);

       } else { // parent

            if (pmatrix->isBackground) {
                //printf("I am a background process\n");
                //waitpid(pid,&status,0);
                //printf("About to check backgroundJobs\n");
                for (i = 0; i < BACKGROUND_MAX; i++) {
                  //store background job in first available slot
                  if (bJobs[i].jobPid == 0) {
                    //printf("About to set backgroundJob\n");
                    bJobs[i].jobPid = pid;
                    //printf("copying command %s to jobs at index %d\n", pmatrix->commandArr[0].command, i);
                    //strcpy(bJobs->pidArray[i], pmatrix->commandArr[0].command);
                    bJobs[i].jobName = cmdLine;
                    //printf("Just set backgroundJob\n");
                    break; //exit loop
                  }
                }
                continue;
            }
            else {
              //printf("I am parent %d, group %d. My son is %d.\n", getpid(), getpgid(getpid()),pid);
              //waitpid(pid,&status,WUNTRACED);
              waitpid(pid,&status,0);
            }
       } //end parent

    } //end new process




      printf("+ completed '%s' [%d]", cmdLine, status);
      for (i = 0; i < pmatrix->num_pipes; i++) {
        printf("[%d]", status);
      }
      printf("\n");
      printf("My pid is %d\n", pid);
      pid = waitpid(-1, &status, WNOHANG);
      if (pid > 0) {
          for (i = 0; i < BACKGROUND_MAX; i++) {
            if (bJobs[i].jobPid == pid) {
              break;
            }
          }
          //printf("I was a background process");
          printf("+ completed '%s' [%d]\n", bJobs[i].jobName, status);
          bJobs[i].jobName = NULL;   // clear that job
          bJobs[i].jobPid = 0;
      }
      free_parseMatrix(pmatrix);
      free(cmdLine);



  }/* while(1) */


}

void executeCommand(parseMatrix *pm)
{
      if (execvp(pm->commandArr[0].command,pm->commandArr[0].argList) < 0) {
          printf("Error: execvp failed");
          exit(1);
        }
}

int catchBuiltInCommand(parseMatrix *pm)
{
      /* get the first commandMatrix command */
      char *command = pm->commandArr[0].command;

      if (strcmp(command,"pwd") == 0) {
        //puts("Trying to get pwd\n");
        s_pwd(pm->commandArr[0].argList);
        return 1;
      }

      else if (strcmp(command,"cd") == 0) {
        //puts("Trying to get cd\n");
        s_cd(pm->commandArr[0].argList);
        return 1;
      }


      else if (strcmp(command,"exit") == 0) {
        //puts("Trying to get exit\n");
        return -1;
      }

      else {
        return 0;
      }
}


int s_cd(char **args){
    if (args[1] == NULL) {
      fprintf(stderr, "Error: no such directory\n");
    } else {
      if (chdir(args[1]) != 0) {
        fprintf(stderr, "Error: no such directory\n");
      }
    }
    return 1;
}

int s_pwd(char **args) {
    char buffer[PATH_MAX + 1];
    char *cwd = getcwd( buffer, PATH_MAX + 1 );
    if( cwd != NULL ) {
      printf("%s\n", cwd);
    }
    return 1;
}

void init_backgroundJobs(backgroundJob bj[])
{
    int i;
    for (i = 0; i < BACKGROUND_MAX; i++) {
      bj[i].jobName = NULL;   // initialize the job name buffer
      bj[i].jobPid = 0;
    }
}

void free_backgroundJobs(backgroundJob bj[])
{
  int i;
  for (i = 0; i < BACKGROUND_MAX; i++) {
    free(bj[i].jobName);
  }

}

void catchPipe(parseMatrix *pm)
{

  /*create array with 2 ends (read and write) allocated to exact number of pipes */
  int fd[pm->num_pipes][2];
  int num_of_child = pm->num_pipes+1;
  pid_t child_pid[num_of_child];


    /* working for single pipe */

    for(int i = 0; i <= pm->num_pipes; i++)
    {
       pipe(fd[i]);
      
       //printf("Number of pipes: %d\n", i);
        if ( ( child_pid[i] = fork() ) == 0) 
        {     

          //printf("Child of pipe: %d\n", i);
          if( i == 0) // first command ever entered
            {
              
            close(fd[i][0]);                     // Don't need to read from pipe         
            dup2(fd[i][1], STDOUT_FILENO);     // Replace stdout with the pipe (write execvp to file descriptor)
            close(fd[i][1]);                   // Close now unused file descriptor   


            }    
      
          else if (i == pm->num_pipes) // last command ever enterd
             {
             close(fd[i-1][1]);                      // Don't need to write to pipe                 
             dup2(fd[i-1][0], STDIN_FILENO);       // And replace stdout with a pipe         
             close(fd[i-1][0]);                   // Close now unused file descriptor 


             }

          else // middle commands
            {
                  close(fd[i - 1][1]); //close the write end of the previous pipe (already finished writing to the pipe)
                  dup2(fd[i - 1][0],STDIN_FILENO); // replace stdinput with the read part of the pipe so we read from pipe rather than standard input
                  close(fd[i - 1][0]); // close the read end of the pipe done reading
                  close(fd[i][0]); // current pipe does not read this pipe will do the execution
                  dup2(fd[i][1],STDOUT_FILENO); // replace stdoutput with the write part of the current pipe (execvp will write to this descriptor)
                  close(fd[i][1]); //close the write end we done hrer

            }


              execvp(pm->commandArr[i].command, pm->commandArr[i].argList); 
          }//child

          //waitpid(child_pid[i], NULL, 0);
      }   //parent

      /*close all of the pipes in the parent */
      for (int h = 0; h <= pm->num_pipes; h++) 
      {
          close(fd[h][0]);
          close(fd[h][1]);
      }

    /*wait for all the children to complete */
    for (int k = 0; k < num_of_child; k++) 
    {
       waitpid(child_pid[k], NULL, 0);

    }
}

/* Handles < > redirection operations */
void catchRedirection(parseMatrix *pm){
    int fileDescriptor0; //For input
    int fileDescriptor1; //For output
    if (pm->hasInfile == 1) { //if we have an indirection
        fileDescriptor0 = open(pm->inFile, O_RDONLY); //open the file, returning it's file descriptor (or -1 on fail)
        if (fileDescriptor0 < 0) { //if there was a problem opening the file, print error and exit
            printf("Error: couldn't open the input file '%s'\n", pm->inFile);
            exit(0);
        }
        dup2(fileDescriptor0, STDIN_FILENO); //swap STDIN with the file descriptor for the input
        close(fileDescriptor0); //close the descriptor as we don't need it anymore
    }
    if (pm->hasOutfile == 1) { //if we have an outdirection
        fileDescriptor1 = creat(pm->outFile , 0644); //open (or create) file with read and write permissions
        //setting the mode of creat to 0644 allows us to read and write to the file
        dup2(fileDescriptor1, STDOUT_FILENO); //swap STDIN with the file descriptor for the output
        close(fileDescriptor1); //close the descriptor as we don't need it anymore
    }
}

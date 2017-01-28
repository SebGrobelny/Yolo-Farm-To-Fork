/* Shawn Taxerman, Itai Ofir,  Sebastian Grobelny

parser.c - source code for parser */

#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

#define MAXLINE 512
#define MAX_COMMAND_LEN 32
#define MAX_PIPES 16



void parse_single_command(char *command, commandMatrix *cm)
{
    int line_pos = 0;
    int word_pos = 0;
    char word[MAXLINE];

    init_commandMatrix(cm);

    /* check if command is empty */
    if (command[line_pos] == '\0')
        return;

    /* skip beginning white space */
    while(isspace(command[line_pos])) {
      line_pos++;
    }

    /* read command and args into the commandMatrix */
    while (command[line_pos] != '\0') {

      /* read non-space chars into the buffer */
      while (!isspace(command[line_pos]) && command[line_pos] != '\0') {
        word[word_pos++] = command[line_pos++];
      }

      word[word_pos] = '\0'; // end the current buffer

      /* Add the command to argList */
      cm->argList[cm->num_args] = malloc((strlen(word)+1)*sizeof(char)); // allocate memory for word
                                                                         //add 1 to account for '\0'
      strcpy(cm->argList[cm->num_args], word);     // copy buffer into array
      cm->num_args++;

      /* clear the word buffer */
      memset(&word[0],'\0',sizeof(word));
      word_pos = 0;
      while(isspace(command[line_pos])) { //skip white space again
        line_pos++;
      }
    }
    /* Copy argLis[0] into the commandMatrix command field */
    cm->command = malloc((strlen(cm->argList[0])+1)*sizeof(char));
    strcpy(cm->command, cm->argList[0]);
    cm->argList[cm->num_args] = NULL; //Terminate argList with NULL

}

int isEmpty( char *line, int i) {
  if (line[i] == '\n' && line[i] == '\0') // no multi-char constants
      return 1;
  else
      return 0;
}

parseMatrix *parse(char *line)
{
    parseMatrix *result = malloc(sizeof(parseMatrix)); // Declare and allocate memoery for the result
    if (result == NULL) { return NULL;}                 // if malloc encounters error, return
    init_parseMatrix(result);                                // initialize the result

    int line_pos = 0;        // tracks position through entire line
    int file_pos = 0;      // tracks position of filename buffer
    int end = 0;       // boolean: checks whether we are at end of line
    int com_pos = 0;       // tracks pos within a single command
    char command[MAXLINE];  // char buffer of current command

    /* check whether user inputs nothing */
    if (isEmpty(line, line_pos))
        return NULL;

    /* Go through the command line */
    while (line[line_pos] != '\n' && line[line_pos] != '\0') //Check written explicitly b/c De Morgan's Laws
    {
      /* check for background command */
      if (line[line_pos] == '&') {
        result->isBackground = 1;
        if (line[line_pos+1] != '\n' && line[line_pos+1] != '\0') {
          fprintf(stderr, "mislocated background sign.\n");
        }
        break;
      }

      /* check for pipe, and parse the command before the | */
      else if (line[line_pos] == '|') {
        command[com_pos] = '\0';         // delimit the current command
        parse_single_command(command, &result->commandArr[result->num_pipes]);
        /* Reset variables to process next command */
        com_pos = 0;
        end = 0;
        result->num_pipes++;
        line_pos++;
      }

      /*check for output redirection */
      else if (line[line_pos] == '>') {

        /* ensure only 1 output file */
        if(result->hasOutfile) {
          fprintf(stderr, "Error. Mislocated output redirection \n");
          free_parseMatrix(result);
          return NULL;
        }

        result->hasOutfile = 1;
        /*skip whitespace */
        while(isspace(line[++line_pos]));
        file_pos = 0;
        /* read in the name of outFile*/
        while(line[line_pos] != '\0' && !isspace(line[line_pos])) {
          if(file_pos == MAX_COMMAND_LEN ) {
            fprintf(stderr, "Error.The output redirection file name exceeds the size limit 32\n");
            free_parseMatrix(result);
            return NULL;
          }

          result->outFile[file_pos++] = line[line_pos++];
        }

        result->outFile[file_pos]='\0';
        end = 1;
        /* skip whitespace until newline*/
        while(isspace(line[line_pos])) {
            if(line[line_pos] == '\n')
              break;

            line_pos++;
          }
      }

      /*check input redirection */
      else if( line[line_pos] == '<') {

        if(result->hasInfile) {
          fprintf(stderr, "Error. Mislocated input redirection \n");
          free_parseMatrix(result);
          return NULL;
        }

        result->hasInfile = 1;
        /*skip whitespace*/
        while(isspace(line[++line_pos]));
        file_pos = 0;
        /* read in the name of inFile*/
        while(line[line_pos] != '\0' && !isspace(line[line_pos])) {
          if(file_pos == MAX_COMMAND_LEN ) {
            fprintf(stderr, "Error.The input redirection file name exceeds the size limit 32\n");
            free_parseMatrix(result);
            return NULL;
          }

          result->inFile[file_pos++] = line[line_pos++];
        }

        result->inFile[file_pos]='\0';
        end = 1;
        /* skip whitespace until newline*/
        while(isspace(line[line_pos])) {
            if(line[line_pos] == '\n')
              break;

            line_pos++;
          }

      }

      /* Parse regular text */
      else {
          if (end == 1) {
           fprintf(stderr, "Error. Wrong format of input\n"); //TODO: write proper message
         	 free_parseMatrix(result);
         	 return NULL;
          }

          if (com_pos == MAXLINE -1) { // subtract 1 to account for '\n'
            fprintf(stderr, "Error. Exceeded character limit\n"); //TODO: write proper message
            free_parseMatrix(result);
            return NULL;
          }

          /* read from the line into the current command buffer */
          command[com_pos++] = line[line_pos++];
      }

    }//end while

    /* terminate the current command buffer */
    command[com_pos]='\0';

    parse_single_command(command, &result->commandArr[result->num_pipes]);
    return result;
}

void print_matrix(parseMatrix *pm)
{
  int cm; /*index of commandMatrix in pm */
  int i; /*index of an argument for specific command matrix*/
  commandMatrix *placeholder;


  if (pm == NULL) {
    printf("null info\n");
    return;
  }


  printf("# of pipes:%d\n", pm->num_pipes);
  /* traverse through commandArr based on number of pipes*/
  for (cm = 0; cm <= pm->num_pipes; cm++)
   {
    placeholder = &(pm->commandArr[cm]);
    if (placeholder == NULL || placeholder->command == NULL) {
        printf("Command %d is NULL\n", cm);
    } else {
      printf("Command %d is %s.\t", cm+1, placeholder->command);
      for (i = 0; i < placeholder->num_args; i++) {
        printf("Arg %d: %s ", i, placeholder->argList[i]);
      }
        printf("\n");
    }
  }
  printf("\n");
  if (pm->hasInfile){
    printf("infile: %s\n", pm->inFile);
  }else {
    printf("no input redirection.\n");
  }
  if (pm->hasOutfile) {
    printf("outfile: %s\n", pm->outFile);
  }else {
    printf("no output redirection.\n");
  }
  if (pm->isBackground){
    printf("Background process.\n");
  } else {
    printf("Foreground process.\n");
  }
}

void init_parseMatrix(parseMatrix *pm)
{
  pm->hasInfile = 0;
  pm->hasOutfile = 0;
  pm->isBackground = 0;
  pm->num_pipes = 0;

  int i;
  for (i = 0; i < MAX_PIPES; i++) {
    pm->commandArr[i].command = NULL;   // initialize the commandMatrix
    pm->commandArr[i].argList[0] = NULL;
    pm->commandArr[i].num_args = 0;
  }
}

void init_commandMatrix(commandMatrix * cm)
{
  cm->command = NULL;
  cm->argList[0] = NULL;
  cm->num_args = 0;
}

void free_parseMatrix (parseMatrix * pm)
{
  int cm; /*index of commandMatrix in pm */

  commandMatrix *placeholder;

  if (pm == NULL) {
    return;
  }

  for (cm = 0; cm < MAX_PIPES; cm++) {
    /* point placholder to a specific commandMatrix*/
    placeholder = &(pm->commandArr[cm]);
    free_commandMatrix(placeholder);
  }
  free(pm);
}

void free_commandMatrix(commandMatrix * cm)
{
  int i;
  for (i = 0; i < cm->num_args; i++) {
    free(cm->argList[i]);
  }
  if (cm->command != NULL) {
    free(cm->command);
  }
}

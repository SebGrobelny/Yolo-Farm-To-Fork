/* Shawn Taxerman, Itai Ofir, Sebastian Grobelny

parser.h - header file for parser */

#ifndef PARSER_H
#define PARSER_H

#define MAX_ARGS 16

/* commandMatrix: Structure to store command information */
typedef struct commandMatrix {
    char *command;           // command specified
    char *argList[MAX_ARGS]; // array of command arguments
    int num_args;            // number of arguments. Also used as index into argList
} commandMatrix;

/* parseMatrix: Structure to store parsing information */
typedef struct parseMatrix {
  int   hasInfile;		       // boolean: input file specified (<)
  int   hasOutfile;		       // boolean: output file specified (>)
  int   isBackground;		     // boolean: background process specified (&)
  int   num_pipes;           // number of pipes specified. Also used as index into commandArr
  char  inFile[40];	         // input file from <
  char  outFile[40];	       // output file to >
  commandMatrix commandArr[16]; // array of commands specified
} parseMatrix;

//PROTOTYPES
void init_parseMatrix(parseMatrix *);
void init_commandMatrix(commandMatrix *);
void free_matrix(parseMatrix *);
void print_matrix(parseMatrix *);
char *read_line(void);
void parse_single_command(char *, commandMatrix * );
int isEmpty(char *, int);
parseMatrix *parse(char *);
void print_matrix(parseMatrix *);
void free_parseMatrix(parseMatrix *);
void free_commandMatrix(commandMatrix *);

#endif /* PARSER_H */

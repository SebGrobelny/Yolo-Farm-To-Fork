#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "exampleParser.h"   /*include declarations for parse-related structs*/




int main (int argc, char **argv)
{


  char * cmdLine;
  parseInfo *info; /*info stores all the information returned by parser.*/
  struct commandType *com; /*com stores command name and Arg list for one command.*/


  while(1){
    /*insert your code to print prompt here*/
    printf("ex# ");


    cmdLine = read_line();
    if (cmdLine == NULL) {
      fprintf(stderr, "Unable to read command\n");
      continue;
    }


    /*insert your code about history and !x !-x here*/

    /*calls the parser*/
    info = parse(cmdLine);
    if (info == NULL){
      free(cmdLine);
      continue;
    }
    /*prints the info struct*/
    print_info(info);

    /*com contains the info. of the command before the first "|"*/
    com=&info->CommArray[0];
    if ((com == NULL)  || (com->command == NULL)) {
      free_info(info);
      free(cmdLine);
      continue;
    }
    /*com->command tells the command name of com
    if (isBuiltInCommand(com->command) == EXIT){
      exit(1);
    }*/

    /*insert your code here.*/

    free_info(info);
    free(cmdLine);
  }/* while(1) */
}


char *read_line(void)
{

    char *line = NULL;
    size_t nbytes = 0;
    ssize_t read = getline(&line,&nbytes,stdin);
    if (read < 0)
    {
      puts ("Error reading line\n");
      return NULL;
    }

    return line;

}

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include "arrow.h"
#include "noncanmode.h"
#include "hstack.h"


char newline = '\n';
char binaryZero = '\0';
char backspace[] = "\b \b"; // two backspaces one to move cursor and the other to remove character
char bell = '\a'; // bell sound
char prompt[] = "8==D ";
//char up = 'u';



char *read_input(hstack *hs)
{

    termios_set_noncanonical();

    char *line = malloc(512*sizeof(char));    //Buffer for the command line
    int pos = 0;                              // Tracks position in STDOUT
    char data;                                // Stores the key read in


    //Print the prompt
    write(STDOUT_FILENO,prompt,sizeof(prompt));

    while( (data = get_one_char()) != 0x0A)
    {


      /*check for Ctrl-D*/
      if(data == 0x04)  {
        termios_reset();
        return NULL;
      }

      /* check for arrow keys */
      if( data == '\033')  {   // check for escape sequence


            get_one_char(); // Skip the '['
            switch(get_one_char()) {   //Get the real value
              case 'A': // UP arrow
                        pos = clearSTDOUT(pos);

                        break;
              case 'B': // DOWN arrow

                        pos = clearSTDOUT(pos);

                      break;
            }
            continue;
        }



      /* check for backspace */
      if (data == 0x7f) {

              if( pos > 0)  {  // Are there characters displayed in STDOUT?

                 write(STDOUT_FILENO, backspace, (sizeof(backspace)));
                 line[--pos] = '\0'; //remove character from line buffer
              }

              else  {         // Reached the prompt. Ring the bell!
                  toSTDOUT(bell);
              }
        continue;
      }

      /* Read in entered character to our line buffer */
      write(STDOUT_FILENO,&data,1);
      line[pos++] = data;

    } //end while (finished reading in command line)

    termios_reset();

    line[pos] = '\0'; //end the line buffer

    /* Store the line in the historyBuffer 
    hb->commandList[hb->h_count] = malloc((strlen(line)+1)*sizeof(char));
    strcpy(hb->commandList[hb->h_count], line);
    hb->h_count++; */

    toSTDOUT(newline); //Print a newline to STD_OUT
    return line;

}



/* Helper function to print to STD_OUT */
void toSTDOUT(char c)
{
    write(STDOUT_FILENO,&c,sizeof(c));
}

int clearSTDOUT(int pos)
{
  while (pos != 0) {
      write(STDOUT_FILENO, backspace, (sizeof(backspace)));//write backspace to STDOUT
      pos--;
  }
  return pos;
}

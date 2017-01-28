#ifndef DATA_H
  #define DATA_H

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

//self-made headers 
#include "Arrow.h"
#include "HandleChildren.h"
#include "Stack.h"

//sets flags for piping if a pipe is found then have to partition current line based on pipe 
// if no pipe is found then process command via execvp so have seperate function for that

// int  parse(char *line, char **argv)
// {
// 	int pipeBaramount = 0;
// 	int pipe>amount = 0;
// 	int pipe<amount = 0;

// 	int pipeAmount = 0;
// 	int array_count = 0;
// 	int pipBarLocation[16][512];
// 	int pip>Flag = 0;
// 	int pip<Flag = 0;

// 	char* pipProcess1;
// 	char* pipProcess2;

// 	//piptype

// 	int pipBar = 1;
// 	int pip> = 2;
// 	int pip< = 3;

//      while (*line != '\0') {       /* if not the end of line ....... */ 
//           while (*line == ' ' || *line == '\t' || *line == '\n'|| *line == '|' ||  *line == '>'  ||  *line == '<')
//           {
//           		if(*line == '|') // write stdout from the leftside into a file descriptor and pipe (dup2)--execute commands using execvp
// 	          		{
// 	          			*line++='\0';

// 	          			pipeLocation[pipeAmount][pipeAmount] = pipBar;
// 	          			pipeLocation[pipeAmount] = array_count;

// 	          			pipeAmount++;


	          			
// 	          		}

//           		if(*line == '>') // write stdout from leftside into an acutal file  (dup2)  
// 		          	{
// 		          		*line++= '\0';

// 		          		pipe_amount++;
// 		          	}

// 		        if(*line == '<') // opposite of above one (dup2)
// 			        {
// 			        	*line++= '\0';

// 			        	pipe_amount++;
// 			        }
// 			    if(pipBarFlag == 0 || pip>Flag == 0 || pip<Flag == 0)
// 				    {
// 				    	*line++ = '\0'; /* replace white spaces with 0    */
// 				    }
                   
// 		   }
//           *argv++ = line;          /* save the argument position     */
// 		  array_count = array_count + 1;
//           while (*line != '\0' && *line != ' ' && 
//                  *line != '\t' && *line != '\n' && *line != '|' &&  *line != '>'  &&  *line != '<') 
//                line++;             /* skip the argument until ...    */
//      }
//      *argv = '\0';                 /* mark the end of argument list  */


//  	return pipe_amount;
// }


int new_parse(char *line)
{
	char *rest = line;

	char *token;

	char *argv[16*32];


	while((token = strtok_r(rest, " ", &rest)))
	{
		if(*token == '|')
	    	printf("token:%s\n", token);
	    if(*token == '>')
	    	printf("token:%s\n", token);
	    if(*token == '<')
	    	printf("token:%s\n",token);
	    strcpy(*argv,token);
	}

	while(*argv != NULL)
	{
		printf("Argument: %s \n", *argv);
		argv++;
	}

}




// data is the most recent character the user enters
// current_line is the user's current entry assuming they haven't hit enter yet
// char_count maintains the count of characters the user currently put into the line
// arrowStore stores the users previous entries
// arrowPose maintains size of arrowStore
int parseData(char data, char *current_line, int char_count, char **arrowStore, int arrowPose )
{


	char  *argv[32];

  	// flags for the backspace
	if( (int)data == 8 || (int)data == 127)
		{

			// character count greater than zero means we can keep deleting off current line
			if( char_count > 0)
			{
				// printf("HIT BACKSPACE");

				char backspace[] = "\b \b";
				
				write(STDOUT_FILENO, backspace, (sizeof(backspace)));
				fflush(stdout);

				//decrement character count
				// char_count = char_count -1;
				//append null to last place
				// current_line[char_count] = '\0';
				return -1;
			}
			//nothing in current line do annoying ping sound 
			else
			{
				char bell[] = "\a";
				printf("Found Bell\n");
				
				write(STDOUT_FILENO, bell, sizeof(bell) );
				return -1;
			}
		}

	// handling escape sequence (this is where arrow keys get dealt with)
	if( (int)data == 27)
		{
		   FoundArrow();
		   

		}

	// handling the newline--someone press enter 
	if( data == 0x0A)
		{
			current_line[char_count] = data;

			char* printable;

			printf("Char count is %d \n", char_count);
			printf("Current line is %s \n", current_line);

			//should copy the entire line into the 2d list at the arrowPose position
			// strcpy(arrowStore[arrowPose], current_line);
			// arrowPose=arrowPose-1;

			// if arrowPose == 0
			// 	arrowPose = 9;
			
			// arrowStore[0]
			int pipe_amount = 0;
			// parseCurrentLine(current_line, char_count);
			 // pipe_amount = parse(current_line, argv);

			new_parse(current_line);

			 // for( int i = 0; i < 64; i++)
		 	// 	{
		 	// 		printf("Argument : %s", argv[i]);

		 	// 	}

			// if(pipe_amount > 0)
			// 	{
			// 		handlePipe(argv, pipe_amount);
			// 	}

			// else
			// 	{
			// 		printf("No pipe found send me into execution\n");

			// 		// execvp()
			// 	}

	
			// push(theStack, current_line);
			// printable = top(theStack);
			// printf("Top of the stack is %s", printable);


			return 1;

		}

	// Ctrl-D is entered
	 if (data == 0x04) 
		 {
			 /* Ctrl-D */
			 return 0;
		 }



    current_line[char_count] = data;
    
  

	return 1;
}




	//  if (isprint(data))
	// 	 {
	// 	 	 printf("RX: '%c' 0x%02X\n", data, data);

	// 	 }
			
	// else
	// 	printf("RX: ' ' 0x%02X\n", data);





// void parseCurrentLine( char *current_line, int char_count)
// {

// 	char **args;

// 	printf("Current Line %s \n", current_line);

// 	int i = 0;
// 	//amount of pipes found in current line
// 	int pipe_amount = 0;
// 	//indexes of the pipes in the current line--so we know how to parse the line in hanlePipe
// 	int *pipeLocations = ( int * )malloc( 2 * sizeof( int ) );

// 	while(*current_line)
// 	{
// 		// printf("Current position %c", *current_line);
// 		if( *current_line == '|' ||  *current_line == '>'  ||  *current_line == '<')
// 			{
// 				printf("Found pipe set flag \n");
// 				//replace pipe with null
// 				// current_line = 'b';
// 				//set a pipe location
// 				// *current_line++ = NULL; 
// 				// *args++ = current_line;

// 				// printf("Made it \n");

// 				// pipeLocations[pipe_amount] = i;
// 				// // //increment pipe counter
// 				// pipe_amount += 1;
// 			}
		
// 		// while (*current_line != '\0')
// 		// 	{
// 		current_line++;
// 			// }
		

// 	}

// 	  *args = NULL;

// 	 printf("Made it here");

// 	if(pipe_amount > 0)
// 	{
// 		handlePipe(args, pipeLocations, pipe_amount, char_count);
// 	}

// 	else
// 	{
// 		printf("No pipe found send me into execution\n");

// 		// execvp()
// 	}

	
// }










#endif
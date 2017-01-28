#include <unistd.h>
#include <termios.h>

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

//self-made headers 
#include "Canonical.h"
#include "Data.h"
#include "Arrow.h"

//maximum character entry
#define MAXCHAR 512

int main()
{

//noncanonical crap
	struct termios SavedTermAttributes;
	termios_set_noncanonical();



//path variables
    char* cwd;
    char buff[PATH_MAX + 1];
    int cwd_size;

//parse data variables
	char data;
	char current_line[MAXCHAR];
	int flags;
	int char_count = 0;


// stack for arrow keys
	// struct stack_t *theStack = newStack();
	char arrowStore[9][512];
	int arrowPose;

	



//display current working directory

	cwd = getcwd(buff, PATH_MAX + 1);
	cwd_size = 	4*(sizeof(cwd)+4);
	char space[] = " ";


	write(STDOUT_FILENO, cwd, cwd_size);
	write(STDOUT_FILENO, space, (sizeof(space)));


		while(1)
	  	{

	  		read(STDIN_FILENO, &data,1);

		  	// c = get_one_char();

	  		flags = parseData(data, current_line, char_count, arrowStore, arrowPose );


	  		//if Ctrl-D entered
			if(flags == 0)
	  		{
	  			break;
	  		}

	  		

	  		//backspace entered
	  		if(flags == -1)
	  		{
	  			char_count = char_count -1;
	  			current_line[char_count] = '\0';
	  			continue;
	  		}

	  		//everything is fine

	  		if(flags == 1)
	  		{
	  			char_count = char_count+1;
	  			write(STDOUT_FILENO, &data, sizeof(data));	
	  		}

			
				 

	  	}


	  termios_reset();

	return 0;


}

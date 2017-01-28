#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "parser.h"




void handlePipe(parseMatrix *pm)
{
	/*create array with 2 ends (read and write) allocated to exact number of pipes */
	int fd[pm->num_pipes][2];
	int num_of_child = pm->num_pipes+1;
	pid_t child_pid[num_of_child];
    int k;

    /* working for single pipe */

 	for(int i = 0; i <= pm->num_pipes; i++)
	{

		pipe(fd[i]);
		
	    if ( ( child_pid[i] = fork() ) != 0) 
	    {     

	    if( i == 0) // first command ever entered
		    {
          
		    close(fd[i][0]);                     // Don't need to read from pipe         
		    dup2(fd[i][1], STDOUT_FILENO);      // Replace stdout with the pipe         
		    close(fd[i][1]);                   // Close now unused file descriptor   
		    //execvp(pm->commandArr[i].command, pm->commandArr[i].argList);  

		    }    

	        
		 
	    
	           
  
	    else if (i == pm->num_pipes) // last command ever enterd
		     {
		     close(fd[i-1][1]);                      // Don't need to write to pipe         
		     //close(STDIN_FILENO);                 //Close existing stdout         
		     dup2(fd[i-1][0], STDIN_FILENO);       // And replace it with the pipe         
		     close(fd[i-1][0]);                   // Close now unused file descriptor 
			//execvp(pm->commandArr[i].command, pm->commandArr[i].argList); 

		     }

		else
		{
          close(fd[i - 1][1]);
          dup2(fd[i - 1][0],STDIN_FILENO);
          close(fd[i - 1][0]);
          close(fd[i][0]);
          dup2(fd[i][1],STDOUT_FILENO);
          close(fd[i][1]);

		}


			    // for (int j = 0; j <= pm->num_pipes; j++) 
			    // {
			    //     close(fd[j][0]);
      	// 			close(fd[j][1]);
			    // }

			    execvp(pm->commandArr[i].command, pm->commandArr[i].argList);
	     }


	}   


    for (int h = 0; h <= pm->num_pipes; h++) 
    {
        close(fd[h][0]);
		close(fd[h][1]);
    }



	for (int k = 0; k < num_of_child; k++) 
	{
	  waitpid(child_pid[k], NULL, WUNTRACED);
	  printf("the %dth child returns\n", k + 1);
	}


	}


   






 







/*inFile and outFile hold filenames*/

// void handleRedirect(parseMatrix *pm)
// {

// 	int in;


// 	if(pm->hasInfile == 1)
// 	{
// 		in = open(inFile, O_RDONLY);	


// 		// replace standard input with input file

// 		dup2(in,0);

// 		close(in)
// 	}

	
/*
    printf("In here");

	for(int i = 0; i < num_of_child; i++)
	{	
		printf("in here");
		pipe(fd[i]);

	    if ( (child_pid[i] = fork()) == 0) 
	    {     

		    if( i == 0) // fist command entered
			    {   
			      // close(fd[i][0]);                  
	      
			    if( dup2(fd[i][1], STDOUT_FILENO) < 0)      
			    {
			    	 perror(" dup2");
	                    exit(EXIT_FAILURE);
			    }      
			    // close(fd[i][1]);                    Close now unused file descriptor   


			    }   
			    

		     
			 else if (i == num_of_child-1) // last command entered
			     {   
			     	close(STDIN_FILENO);
				    if( dup(fd[i-1][0]) < 0) {
		                    perror(" dup2");///j-2 0 j+1 1
		                    exit(EXIT_FAILURE);

		                }
	 
			     }

			 else
			 {
			 	dup2(fd[i][0],fd[i-1][1]); 
			 	dup2(fd[i][1], STDOUT_FILENO); 

			 }

				

			    for (int j = 0; j < pm->num_pipes; j++) 
			    {
			           close(fd[j][0]);
      					close(fd[j][1]);
			    }



				execvp(pm->commandArr[i].command, pm->commandArr[i].argList);
			    
		 


    		}  

    	else
    	{

	         perror("error");
	         exit(EXIT_FAILURE);
   		 } 

	}

	     for (int k = 0; k < num_of_child; k++) {
	      waitpid(child_pid[k], NULL, WUNTRACED);
	      printf("the %dth child returns\n", k + 1);
	    }




	for (int jk = 0; jk < pm->num_pipes; jk++) {
		      close(fd[jk][0]);
		      close(fd[jk][1]);
		    }
*/

  
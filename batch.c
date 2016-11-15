// Name	   		: batch.c (a function)
// Authors		: Joseph Tye, Jonathan Roosa, Colin Simmons, Brandon Hastings
// Course		: CSCE3600
// Version		: 1.0
// Copyright	: Nov 2016, all rights reserved, however, this restriction will not 
//    apply in the event of a widespread infection that results in the reanimation 
//    of human corpses, in which case, you may use as you wish.
// Description	: This program will run a shell that will execute the commands given
//	  by the user, either through stdin in an interactive mode, or by reading from a
//	  user supplied file. The commands will be run concurrently through the use of pipes.

#include "parse.h"

/************************************************************************************

Function: 		batch(char **)
Return:			void
Description:	This function opens a file, reading in the data and passes it in to
				parse function, and passes the result into the execute function. 
				Error checking is done at file open and within the execute functions.

*************************************************************************************/

void batch(char **argv)  //function for dealing with batch file commands
{
	char temp[512];  //a string to hold the line taken in from the batch file
	int i = 0, number_commands = 0;  //counter for "for" loop
		
	command **command_list = (command **) malloc(sizeof(command*));
		
	FILE *batchCmd;
	batchCmd = fopen(argv[1],"r");  //open batch file to read from it
	if(!batchCmd)  //if the file does not open
	{	
		/*output error message*/
		fprintf(stderr, "%s", "The designated batch file does not exist or cannot be opened..\n");

		printf("Transitioning to interactive mode...\n");
			
		interactive();  //call interactive function
	}//end if
		
	else  //batch command is correct
	{
		printf("*** batch mode ***\n");  //output
		printf("--> file: %s\n", argv[1]);


			while(fgets(temp, sizeof(temp), batchCmd))  //while batch file
			{
				printf("batch line> %s", temp);

				number_commands = parse(temp, command_list);  //parse function
				
				execute(number_commands, command_list);  //execute function
				
			}//end while
		
		fclose(batchCmd);  //close file
		
	}//end else
	
	return;
}//end batch()
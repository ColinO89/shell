// Name	   		: batch.c (a function)

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

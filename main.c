// Name	   		: main.c

#include "parse.h"

int main(int argc, char *argv[])
{
	/*declaration of the command list*/
	command **command_list = (command **) malloc(sizeof(command*));
	int number_commands;
	
	if(argc == 2)  //if there is a user supplied batch file
		batch(argv);
	
	else if(argc == 1)  //if no batch file, call interactive mode
		interactive();
	
	else  //if wrong number of command line arguments
		fprintf(stderr, "%s", "Incorrect number of command line arguments, the program will now exit..\n");
		
  return 0;
}


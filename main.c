// Name	   		: main.c
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


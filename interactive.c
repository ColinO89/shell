#include "parse.h"

/************************************************************************************

Function: 		signalTrap(int)
Return:			void
Description:	This function catches the signal interrupt to prevent the shell from
				exiting without the proper command.

*************************************************************************************/

void signalTrap(int sigT)  //function for trapping the signal interupts
{
	if (sigT == SIGINT)
		NULL;
}//end signalTrap

typedef struct  commands{ //holds 255 bytes of memory for a single line of input
	char commands[255];
}commands;

void interactive(){ //funciton to handle the interactive functionality
	signal(SIGINT, signalTrap);  //signal trap

	char prompt[255] = "prompt>"; //initialize default prompt 
	char CL[512]; //entered by the user
	char *eof;
	commands command_history[255]; //stores 255 commands 

	int command_counter = 0; //for saving history
	
	command **command_list  = (command**) malloc(sizeof(command*));
	int num_of_commands = 0;
	int i = 0;

	//greeting
	printf("*** interactive mode ***\n");
	printf("--> type \"prompt\" to change prompt\n");
	printf("--> type \"history\" to display history\n");
	
	//interactive body
	while(1){
		printf("%s ", prompt); //user prompt
		eof = fgets(CL, 512, stdin);
		if (eof == NULL)
				exit(1);
			
		while(strlen(CL) <= 1)  //for dealing with empty input lines
		{
			printf("%s ", prompt);  //reprint the prompt
			eof = fgets(CL, 512, stdin);
			if (eof == NULL)
				exit(1);
		}
		CL[strlen(CL) - 1] = '\0';
		
		if(strcmp(CL, "prompt") == 0){ //user enters prompt
			printf("enter new prompt:\n");

			//read input
			eof = fgets(CL, 512, stdin);
			if (eof == NULL)
				exit(1);
			CL[strlen(CL) - 1] = '\0'; //remove endline character
			strcpy(prompt, CL); //copy prompt

			printf("prompt set...\n");
			
			//save command to history
			strcpy(command_history[command_counter].commands,"prompt");
			command_counter++;
		}//end of prompt

		else if(strcmp(CL, "history") == 0)
		{ //print history
			int i;
			for(i=0; i<command_counter; i++){
				printf("%s\n", command_history[i].commands); //print history entry
			}//end of for
			
			//save to history
			strcpy(command_history[command_counter].commands,"history");
			command_counter++;
		}//end of history
		else if(strcmp(CL, "quit") ==0){
			exit(EXIT_SUCCESS);
		}
		else if(strcmp(CL, "exit") == 0){
			exit(EXIT_SUCCESS);
		}
		else{ //commands entered
			num_of_commands = 0;
			
			num_of_commands = parse(CL, command_list); //parse command that was entered
			
			execute(num_of_commands, command_list); //run commands
			
			int j;

			for(j=0; j<num_of_commands; j++){
			
				strcpy(command_history[command_counter].commands,command_list[j]->arg_list[0]);
				command_counter++; //increment counter to store in correct order
			}			

			// check if the user entered quit
			for(i; i < num_of_commands; i++)
			{
				if(strcmp(command_list[i]->arg_list[0], "quit") == 0)
					return;
				if(strcmp(command_list[i]->arg_list[0], "exit") == 0)
					exit(EXIT_SUCCESS);
			}
		}//end else
	}//end while
}


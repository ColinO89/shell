/********************************************
 *	Command Execution
 *	
 ********************************************/
#include "parse.h"

/************************************************************************************

Function: 		handle status(int, command, int)
Parameters:		two integers and command
Return:			integer value
Description:	This function does a couple string comparrisons on a few special
				commands and sets a status flag accordingly. After going through
				the checks it enters a switch statement to handle the appropriately
				status flag.

*************************************************************************************/
int handle_status(int status, command **command_list, int i)
{

	if(strcmp(command_list[i]->arg_list[0], "cd")==0)
		status = 2;
	else
		status = 7;
	
	int ret;
	switch(status)
	{
		case 2: ret = chdir(command_list[i]->arg_list[1]);
				if(ret != 0)
					printf("error: change directory to %s failed: invalid directory\n",command_list[i]->arg_list[1]);					
				break;
		default: return status;
	}
}
/************************************************************************************

Function: 		execute(int, command)
Parameters:		one integer and command
Return:			void
Description:	This function handles the commands coming in for both interactive 
				and batch mode. Initially it checks if the command coming in has no 
				pipe associated with it, and if not executes the command. If it does
				it goes and uses dup2 to handle executing the pipped command.

*************************************************************************************/
void execute(int number_commands, command **command_list)
{
	int i = 0, j =0;
	int fd[2];
	pid_t pid, pid2;
	
	/*going to loop through all the commands*/
	for(i; i < number_commands; i++)
	{
		if(command_list[i]->pipe_exists == 0)
		{
			pid = fork();	
			
			if(pid == 0)
			{
				execvp(command_list[i]->arg_list[0], command_list[i]->arg_list);
				exit(3);
			}
			else if(pid < 0)
				perror("Error!");
			else
			{
				/* Wait on the child process to finish its tasks first */
				wait(NULL);
				/* random status set, just didnt want to set = 0 */
				int status = 7;
				
				/* The child process has completed its tasks, now it's
				 * time to see if there are any "special" commands we 
				 * need to handle i.e. cd or exit. This step
				 * has to be done in the parent process. If it were done 
				 * in the child each command that was done would essentially
				 * be reset as soon as we returned to the interactive function
				 * and called execute() again.
				 *
				 * */
				for(j; j < number_commands; j++)
				{
					if(status == 7)
						status = handle_status(status, command_list, j);
					else 
						break;
				}
	
			}
		}
		/* The pipe !=0 which implies there is a pipe
		 * so we need to create a pipe, and create two
		 * child processes to handle the arguments coming 
		 * in
		 * 
		 * */
		else
		{
			if (pipe(fd) == -1) 
			{ 
				/* generate the pipe */
				perror("Pipe");
				exit(1); 
			}
			pid = fork();
			if(pid == 0)
			{
				/* In the first child */
				dup2(fd[1], fileno(stdout));
				close(fd[0]);
				close(fd[1]);	
				execvp(command_list[i]->arg_list1[0], command_list[i]->arg_list1);
				perror("Error2!");
				exit(3);
			}
			
			/*Back in parent*/
			pid2 = fork();
			if(pid2 == 0)
			{
				/* In the second child */
				dup2(fd[0], fileno(stdin));
				close(fd[0]);
				close(fd[1]);
				execvp(command_list[i]->arg_list2[0],command_list[i]->arg_list2);	
				perror("Error3!");
				exit(4);			
			}
			/* Closing both ends of the pipe again
			 * to prevent any data getting sent to
			 * the wrong place later
			 *
			 * */
			close(fd[0]);
			close(fd[1]);
			wait(NULL);
			wait(NULL);
		}
	}
}


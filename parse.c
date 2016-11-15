/* COMMAND PARSING SCRIPT
* */

#include "parse.h"

int parse(char *command_line, command **command_list)
{
	/* The command buffer holds the formatted command list string.
	 * The command set buffer holds the current command set. These are
	 * separated by semicolons.
	 * The command term buffer holds the current command in the set.
	 * So if the set is "ls -al | wc", then the second term in that set
	 * (1th term) is "-al" 
	 * 
	 * The save pointers are important for strtok_r
	 * 
	 * */
	/* Set to 520 for extra buffer, last minute hacky fix, sorry */
	char command_buffer[520] = {'\0'};
	char *commandset_buffer, *set_save_ptr;
	char *commandterm_buffer, *term_save_ptr;
	
	/* Variables for the number of commands in the current command line,
	 * as well as the number of command terms per separate command */
	int number_commands = 0;
	int number_command_terms = 0;
	int i, j, pipe_position;
	
	/* Strip out duplicate semicolons and spaces. Run the algo
	 * twice to further strip out any edge cases, such as: ; ; ; */
	for (i = 0; i < 2; ++i) {
		int i_line = 0;
		int i_buffer = 0;
		char current;
		do {
			/* Get character from the command line */
			current = command_line[i_line];
			
			/* If this isn't the char 0, then check if it's 
			 * a space or a semicolon. If it is, then ignore it.
			 * Otherwise, save the character into the buffer and
			 * increment the buffer iterator. */
			if (i_line > 0) {
				/* SPACE CHARACTER CHECKS */
				if (command_line[i_line] == ' ') { 
					/* Is the previous character not a space? */
					if (command_line[i_line - 1] != ' ') { 
						/* Is there not a semicolon on either side? */
						if (command_line[i_line - 1] != ';' && command_line[i_line + 1] != ';') {
							command_buffer[i_buffer] = current;
							i_buffer++;
						}
					}		
				}
				/* SEMICOLON CHARACTER CHECKS */
				else if (command_line[i_line] == ';') { 
					/* Is the previous character not a space? */
					if (command_line[i_line - 1] != ';') {
						/* Is the next character not null? */
						if (command_line[i_line + 1] != '\0') {
							command_buffer[i_buffer] = current;
							i_buffer++;
						}
					}
				}
				/* If the character is neither a semicolon nor space BUT is also not a newline, then add it to the buffer */
				else if (command_line[i_line] != ' ' && command_line[i_line] != ';' && command_line[i_line] != '\n') {
					command_buffer[i_buffer] = current;
					i_buffer++;
				}
			}
			else {
				/* Increment i_line until first non space and non semicolon character */
				while(command_line[i_line] == ' ' || command_line[i_line] == ';') {
					i_line++;
				}
				command_buffer[i_buffer] = command_line[i_line];
				i_buffer++;
			}
	
			/* Increment the line iterator so it can check the
			 * next character to be potentially added to the buffer */
			i_line++;
		} while(current != '\0');

		/* Before the second run through of the algorithm */
		if (i == 0) {
			strcpy(command_line, command_buffer);
			memset(command_buffer, '\0', sizeof(command_buffer));
		}
	}

	/* Allocate number of commands. Find this number by counting the
	 * number of semicolons in the command buffer and then adding 1 */
	for (i = 0; i < strlen(command_buffer); ++i) {
		if (command_buffer[i] == ';')
			number_commands++;
	}
	number_commands++;
	
	
	/* Allocate the number of commands as a command structure array */
	for (i = 0; i < number_commands; ++i) {
		command_list[i] = (command *) malloc(sizeof(command) * 20);
	}
	
	/* Test commands for dynamic allocation
	command_list[1] -> arg_list = malloc(5 * sizeof(char *));
	command_list[1] -> arg_list[1] = malloc(20 * sizeof(char));
	strcpy(command_list[1] -> arg_list[1], "Test String");
	* */
	
	/* Use strtok() on each command list set in the command buffer, and
	 * store it in the commandset_buffer */
	commandset_buffer = strtok_r(command_buffer, ";", &set_save_ptr);
	for (i = 0; i < number_commands; ++i) {
		/* Count number of command terms in the current command set
		 * of the whole command line buffer. Do this by counting number
		 * of spaces and then adding one */
		number_command_terms = 0;
		for (j = 0; j < strlen(commandset_buffer); ++j) {
			if (commandset_buffer[j] == ' ')
				number_command_terms++;
		}
		number_command_terms++;
		
		/* Now use that number to dynamically allocate the argument
		 * list for the current command list set. So if the command is
		 * "ls -al | wc", then this will allocate space for 4 strings
		 * in the ith command list set. Make sure to set the final
		 * argument to NULL. Important for execvp */
		command_list[i] -> arg_list = malloc(number_command_terms * sizeof(char *) + 1);
		command_list[i] -> arg_list[number_command_terms - 1] = malloc(sizeof(char));
		strcpy(command_list[i] -> arg_list[number_command_terms - 1], "\0");
		command_list[i] -> pipe_exists = 0;
		pipe_position = 0;
		
		
		/* Finally, strtok each term in the current set buffer. Each term
		 * will be stored in the arg list of the current command list set */		
		commandterm_buffer = strtok_r(commandset_buffer, " ", &term_save_ptr);
		for (j = 0; j < number_command_terms; ++j) {
			command_list[i] -> arg_list[j] = malloc(sizeof(char) * strlen(commandterm_buffer) + 1);
			strcpy(command_list[i] -> arg_list[j], commandterm_buffer);
			
			/* Pipe handling. If any of the terms is a pipe, signify
			 * it as such, so arg_list1 and arg_list2 can be allocated
			 * and filled properly. */
			if (command_list[i] -> arg_list[j][0] == '|') {
				command_list[i] -> pipe_exists = 1;
				pipe_position = j;
			}
			
			/* Load next term into the buffer */
			commandterm_buffer = strtok_r(NULL, " ", &term_save_ptr);
		}
		
		/* Before moving onto the next set, make sure to check for the
		 * existance of a pipe. If a pipe exists, then split all
		 * arguments before it into arg_list1, and all after it into
		 * arg_list2 */
		if (command_list[i] -> pipe_exists == 1) {
			/* Allocate arg_list1 */
			command_list[i] -> arg_list1 = malloc(pipe_position * sizeof(char *) + 1);
			command_list[i] -> arg_list1[pipe_position] = malloc(sizeof(char));
			command_list[i] -> arg_list1[pipe_position] = NULL;
			
			/* Allocate arg_list2 */
			command_list[i] -> arg_list2 = malloc((number_command_terms - pipe_position) * sizeof(char *));
			command_list[i] -> arg_list2[(number_command_terms - pipe_position) - 1] = malloc(sizeof(char));
			command_list[i] -> arg_list2[(number_command_terms - pipe_position) - 1] = NULL;
			
			/* Copy terms for arg_list1 */
			for (j = 0; j < pipe_position; ++j) {
				command_list[i] -> arg_list1[j] = malloc(sizeof(char) * strlen(command_list[i] -> arg_list[j]) + 1);
				strcpy(command_list[i] -> arg_list1[j], command_list[i] -> arg_list[j]);
			}
			
			/* Copy terms for arg_list2 */
			for (j = pipe_position + 1; j < number_command_terms; ++j) {
				command_list[i] -> arg_list2[j - pipe_position - 1] = malloc(sizeof(char) * strlen(command_list[i] -> arg_list[j]) + 1);
				strcpy(command_list[i] -> arg_list2[j - pipe_position - 1], command_list[i] -> arg_list[j]);
			}
		}
		
		/* strtok next command list set into the commandset buffer */
		commandset_buffer = strtok_r(NULL, ";", &set_save_ptr);
	}
	
	return number_commands;
}

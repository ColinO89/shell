#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
typedef struct {
	int pipe_exists;
	char **arg_list;
	char **arg_list1;
	char **arg_list2;
} command;

int parse(char *command_line, command **command_list);

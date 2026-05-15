#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calculator.h"

Registry registry = {0};

Args readLine(char *line) {
	
	Args args = {0};
	char *tok;
	
	tok = strtok(line, " \t\n");
	
	while(tok != NULL) {
		
		args.arg_v = realloc(args.arg_v, sizeof(char*) * (args.arg_c + 1));
		
		args.arg_v[args.arg_c] = tok;
		args.arg_c++;
		
		tok = strtok(NULL, " \t\n");
	}
	
	return args;
}

int runCommand(Args args) {
	
	int i;
	int exit_val;
	
	for(i = 0; i <= COMMAND_CT; i++) {
		
		if(i == COMMAND_CT) return 1;
		
		if(strcmp(cmds[i].name, args.arg_v[0]) == 0) break;
		
	}
	
	exit_val = cmds[i].function(args);
	
	return exit_val;
}
#include <stdio.h>
#include <stdlib.h>
#include "calculator.h"

int main(void) {
	
	char input[500];
	Args args = {0};
	
	int exit_val;
	
	for(int i = 0; i < COMMAND_CT; i++) {
		printf("%s - %s\n", cmds[i].name, cmds[i].desc);
	}
	
	puts("\n");
	
	while(1) {
		
		fgets(input, sizeof(input) - 1, stdin);
		if (input[0] == '\n') continue;
		
		args = readLine(input);
		
		exit_val = runCommand(args);
		
		if(exit_val == 1) puts("\nERROR\n");
		
		if(exit_val == -1) return 0;
		
		free(args.arg_v);
		args = (Args){0};
	}
}
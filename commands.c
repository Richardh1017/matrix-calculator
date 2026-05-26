#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calculator.h"

Command cmds[COMMAND_CT] = {
	
	//BASIC
	{"help", "lists all functions and brief desc", cmd_help},
	{"exit", "closes calculator\n", cmd_exit},
	
	//MATRIX REGISTRY
	{"create", "creates matrix, syntax: create [ROWS] [COLS]", cmd_create},
	{"delete", "deletes matrix, syntax: delete [POSITION] or delete all to delete every matrix", cmd_delete},
	{"print", "prints matrix, syntax: print [POSITION] or print all to print every matrix", cmd_print},
	{"list", "lists registry compactly\n", cmd_list},
	
	//MATRIX MATH
	{"add", "add 2 matrices together, syntax: add [POSITION 1] [POSITION 2]", cmd_add},
	{"subtract", "subtract 2 matrices, syntax: subtract [POSITION 1] [POSITION 2]", cmd_subtract},
	{"kmult", "multiply matrix by constant k, syntax: kmult [POSITION] [k]", cmd_kmult},
	{"multiply", "matrix multiplication, syntax: multiply [POSITION 1] [POSITION 2]", cmd_multiply},
	{"transpose", "transpose matrix, syntax: transpose [POSITION]\n", cmd_transpose},
	
	//HARDER MATH
	{"rref", "get RREF of matrix using Gaussian-Jordan elimination, syntax: rref [POSITION]\n", cmd_rref}
};


//BASIC
int cmd_help(Args args) {
	
	(void)args;
	
	for(int i = 0; i < COMMAND_CT; i++) {
		printf("%s - %s\n", cmds[i].name, cmds[i].desc);
	}
	
	puts("");
	
	return 0;
}

int cmd_exit(Args args) {
	
	(void)args;
	
	//need to free each matrix in registry, run delete for i = register ct, then free the registry
	
	for(int i = registry.count - 1; i >= 0; i--) {
			
			deleteMatrix(&registry.data[i]);
			
			registry = decreaseReg(registry, i);
			printf("Matrix %d deleted\n", i);
			
		}
	
		free(registry.data);
		printf("Registry deleted\n");
	
	return -1;
}

//MATRIX REGISTRY 
int cmd_create(Args args) {
	
	if (args.arg_c < 3) return 1;
	
	int r = atoi(args.arg_v[1]);
	int c = atoi(args.arg_v[2]);
	
	if (r <= 0 || c <= 0) return 1;
	
	Matrix m = createMatrix(r, c);
	
	fillMatrix(&m);
	
	////////////
	
	registry = increaseReg(registry);
	
	registry.data[registry.count - 1] = m;
	
	return 0;
}

int cmd_delete(Args args) {
	
	if (args.arg_c < 2) return 1;
	
	if (strcmp(args.arg_v[1], "all") == 0) {
		
		for(int i = registry.count - 1; i >= 0; i--) {
			
			deleteMatrix(&registry.data[i]);
			
			registry = decreaseReg(registry, i);
			printf("Matrix %d deleted\n", i);
			
		}
	
		free(registry.data);
		printf("Registry deleted\n");
	
	} else {
		
		int pos = atoi(args.arg_v[1]);
		if (pos < 0 || pos > (registry.count - 1)) return 1;
		
		deleteMatrix(&registry.data[pos]);
		
		registry = decreaseReg(registry, pos);
		printf("Matrix %d deleted\n", pos);
		
	}
	
	puts("");
	
	return 0;
}

int cmd_print(Args args) {
	
	if (args.arg_c < 2) return 1;
	
	if(registry.count == 0) return 1;
	
	if (strcmp(args.arg_v[1], "all") == 0) {
		
		for (int i = 0; i < registry.count; i++){
			
			printf("Matrix %d:\n", i);
			printMatrix(registry.data[i]);
			
		}
		
	} else {
		
		int pos = atoi(args.arg_v[1]);
		
		if(pos < 0 || pos > (registry.count - 1)) return 1;
		
		printMatrix(registry.data[pos]);
		
	}
	
	return 0;
}

int cmd_list(Args args) {
	
	(void)args;
	
	if(registry.count == 0) return 1;	
	
	for (int i = 0; i < registry.count; i++){
		
		printf("[%d] %d x %d\n", i, registry.data[i].rows, registry.data[i].cols);
		
	}
	
	puts("");
	
	return 0;
}

int cmd_add(Args args) {
	
	if(args.arg_c < 3) return 1;
	
	int pos1 = atoi(args.arg_v[1]);
	int pos2 = atoi(args.arg_v[2]);
	
	if (pos1 < 0 || pos1 >= registry.count) return 1;
	if (pos2 < 0 || pos2 >= registry.count) return 1;
	
	//dimension mismatch lmao its so ugly
	if(registry.data[pos1].rows != registry.data[pos2].rows || registry.data[pos1].cols != registry.data[pos2].cols) return 1;
	
	Matrix result = add(registry.data[pos1], registry.data[pos2]);
	
	printMatrix(result);
	puts("");
	
	saveQuestion(result);
	deleteMatrix(&math_buffer);
	
	return 0;
}

int cmd_subtract(Args args) {
	
	if(args.arg_c < 3) return 1;
	
	int pos1 = atoi(args.arg_v[1]);
	int pos2 = atoi(args.arg_v[2]);
	
	if (pos1 < 0 || pos1 >= registry.count) return 1;
	if (pos2 < 0 || pos2 >= registry.count) return 1;	
	
	//dimension mismatch lmao its so ugly
	if(registry.data[pos1].rows != registry.data[pos2].rows || registry.data[pos1].cols != registry.data[pos2].cols) return 1;
	
	Matrix result = subtract(registry.data[pos1], registry.data[pos2]);
	
	printMatrix(result);
	puts("");
	
	saveQuestion(result);
	deleteMatrix(&math_buffer);
	
	return 0;
}

int cmd_kmult(Args args) {
	
	if(args.arg_c < 3) return 1;
	
	int posA = atoi(args.arg_v[1]);
	int k = atoi(args.arg_v[2]);
	
	if (posA < 0 || posA >= registry.count) return 1;	
	
	Matrix result = kmult(registry.data[posA], k);
	
	printMatrix(result);
	puts("");
	
	saveQuestion(result);
	deleteMatrix(&math_buffer);
	
	return 0;
}

int cmd_multiply(Args args) {
	
	if(args.arg_c < 3) return 1;
	
	int pos1 = atoi(args.arg_v[1]);
	int pos2 = atoi(args.arg_v[2]);
	
	if (pos1 < 0 || pos1 >= registry.count) return 1;
	if (pos2 < 0 || pos2 >= registry.count) return 1;
	
	if(registry.data[pos1].cols != registry.data[pos2].rows) return 1;
	
	Matrix result = multiply(registry.data[pos1], registry.data[pos2]);
	
	printMatrix(result);
	puts("");
	
	saveQuestion(result);
	deleteMatrix(&math_buffer);
	
	return 0;
}

int cmd_transpose(Args args) {
	
	if(args.arg_c < 2) return 1;
	
	int pos = atoi(args.arg_v[1]);
	
	if (pos < 0 || pos >= registry.count) return 1;
	
	Matrix result = transpose(registry.data[pos]);
	
	printMatrix(result);
	puts("");
	
	saveQuestion(result);
	deleteMatrix(&math_buffer);
	
	
	return 0;
}

int cmd_rref(Args args) {
	
	if(args.arg_c < 2) return 1;
	
	int pos = atoi(args.arg_v[1]);
	
	if (pos < 0 || pos >= registry.count) return 1;
	
	Matrix result = rref(registry.data[pos]);
	
	printMatrix(result);
	puts("");
	
	saveQuestion(result);
	deleteMatrix(&math_buffer);
	
	
	return 0;
}
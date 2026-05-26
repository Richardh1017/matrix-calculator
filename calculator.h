#ifndef CALCULATOR_H
#define CALCULATOR_H

typedef struct {
	
	int arg_c;
	char **arg_v;
	
} Args;

typedef int (*cmd_fn)(Args);

typedef struct {
	
	char *name;
	char *desc;
	cmd_fn function;
	
} Command;

#define COMMAND_CT 12

#define EPSILON 1e-7

extern Command cmds[COMMAND_CT];

///////////////////////////////////////////

typedef struct {
	
	int rows;
	int cols;
	float **data;
	
} Matrix;

typedef struct {
	
	int count;
	Matrix *data;
	
} Registry;

extern Registry registry;

extern Matrix math_buffer;

////////////////matrix.c

Registry increaseReg(Registry r);

Registry decreaseReg(Registry r, int pos);

Matrix createMatrix(int row, int col);

void deleteMatrix(Matrix *m);

void fillMatrix(Matrix *m);

void printMatrix(Matrix m);

////////////////basicops.c

Matrix add(Matrix a, Matrix b);

Matrix subtract(Matrix a, Matrix b);

Matrix kmult(Matrix a, float k);

Matrix multiply(Matrix a, Matrix b);

Matrix transpose(Matrix a);

Matrix ref(Matrix a);

void saveQuestion(Matrix result); //idk its just here

////////////////advancedops.c

void scale_row(Matrix *a, float k, int row);

void add_row(Matrix *a, int rA, int rB);

void swap_row(Matrix *a, int rA, int rB);

int find_pivot(Matrix *a, int col, int currentpivot);

int arrange_pivot(Matrix *a, int col, int pivotrow);

void eliminate_column(Matrix *a, int col, int pivotrow);

void reduce_row(Matrix *a, int row);

Matrix rref(Matrix a);

////////////////commands.c

int cmd_help(Args args);

int cmd_exit(Args args);

int cmd_create(Args args);

int cmd_delete(Args args);

int cmd_print(Args args);

int cmd_list(Args args);

int cmd_add(Args args);

int cmd_subtract(Args args);

int cmd_kmult(Args args);

int cmd_multiply(Args args);

int cmd_transpose(Args args);

int cmd_rref(Args args);

////////////////calculator.c

Args readLine(char *line);

int runCommand(Args args);

#endif
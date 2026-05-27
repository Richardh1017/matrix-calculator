#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "calculator.h"

/////////////////////////basic row operations

void scale_row(Matrix *a, float k, int row) {
	
	for(int i = 0; i < a->cols; i++) {
		
		a->data[row][i] *= k;
		
		if (fabs(a->data[row][i]) < EPSILON) a->data[row][i] = 0;
		
	}
}

void add_row(Matrix *a, int rA, int rB) {
	
	//rA + rB
	
	for(int i = 0; i < a->cols; i++) {
		
		a->data[rA][i] += a->data[rB][i];
		
		if(fabs(a->data[rA][i]) < EPSILON) a->data[rA][i] = 0;
		
	}
	
}

void swap_row(Matrix *a, int rA, int rB) {
	
	float temp = 0;
	
	for(int i = 0; i < a->cols; i++) {
		
		temp = a->data[rA][i];
		
		a->data[rA][i] = a->data[rB][i];
		
		a->data[rB][i] = temp;
	}
}

/////////////////////////RREF helpers

int find_pivot(Matrix *a, int col, int trackingpivot) {
	
	int pivotrow = 0;
	
	//to be a pivot, number must be nonzero and under trackingpivot row (trackingpivot row starts at -1 so the first pivot will be found at row 0, which is row 1 ig)
	
	for(int i = 0; i < a->rows; i++) {
		
		if ((fabs(a->data[i][col]) < EPSILON) || (pivotrow <= trackingpivot)) {
			
			pivotrow++;
			continue;
		}
		
		break;
	}
	
	//if(pivotrow < a->rows) printf("pivotrow is R%d\n", pivotrow);
	
	return pivotrow;
}

int arrange_pivot(Matrix *a, int col, int trackingpivot) {
	
	int pivotrow = find_pivot(a, col, trackingpivot);
	
	//if no pivot then pivotrow = a->rows. return 1 to skip column
	if(pivotrow >= a->rows) return 1;
	
	//if pivot isnt in the right row, swap. trackingpivot is + 1 because it doesnt update until after this function, bad design lol
	if(pivotrow != (trackingpivot + 1)) {
		
		printf("R%d <-> R%d:\n", pivotrow, trackingpivot + 1);
		swap_row(a, pivotrow, trackingpivot + 1);
		
		printMatrix(*a);
	}

	return 0;
}

void eliminate_column(Matrix *a, int col, int row) {
	
	float val = a->data[row][col];
	float k = 0;
	
	for(int i = 0; i < a->rows; i++) {
		
		//skip past the pivot row, but eliminate above and below. and make sure you dont eliminate a row thats already 0
		if(i == row || a->data[i][col] == 0) continue;
		
		//scale row to eliminate others
		k = (a->data[i][col] / val);
		scale_row(a, k, row);
		
		//subtracting or adding logic
		if (fabs(a->data[row][col] + a->data[i][col]) > EPSILON) {
			
			scale_row(a, -1, row);
			k *= -1;
		}
		
		add_row(a, i, row);
		
		//revert row scaling
		
		scale_row(a, (1/k), row);
		
		printf("eliminated R%d with R%d:\n", i, row);
		printMatrix(*a);
		
	}
}

void reduce_row(Matrix *a, int row) {
	
	float k = 0;
	
	for(int i = 0; i < a->cols; i++) {
		
		if(fabs(a->data[row][i]) < EPSILON) continue;
		
		k = a->data[row][i];
		break;
	}
	
	if(fabs(k) > EPSILON) scale_row(a, (1/k), row);
}

//only works on already RREFed matrices
int singularity_check(Matrix a) {
	
	for(int i = 0; i < a.rows; i++) {
		
		if(fabs(a.data[i][i]) < EPSILON) return 1; //singular
	}
	
	return 0; //nonsingular
}

/////////////////////////RREF

Matrix rref(Matrix a) {
	
	//set to -1 because the first pivot found will set it to row 0
	int trackingpivot = -1;
	
	//duplicate user matrix
	
	math_buffer = createMatrix(a.rows, a.cols);
	
	for(int i = 0; i < a.rows; i++) {
		
		for(int k = 0; k < a.cols; k++) {
			
			math_buffer.data[i][k] = a.data[i][k];
			
		}
	}
	
	//column by column row reducing, i is the current column
	
	for(int i = 0; i < a.cols; i++) {
		
		//first arrange pivot
		
		  //if no pivot in column, skip
		if(arrange_pivot(&math_buffer, i, trackingpivot) == 1) {
			
			printf("no pivot in column\n\n");
			continue;
		}
		
		//even if it returned 0, the function was still called in the if, dont do it twice
		trackingpivot++;
		printf("pivot tracking = R%d\n\n", trackingpivot);
		
		//eliminate other rows, trackingpivot now equals the row w the pivot
		eliminate_column(&math_buffer, i, trackingpivot);
		
	}
	
	//now put all leading 1s
	
	for(int i = 0; i < a.rows; i++) {
		reduce_row(&math_buffer, i);
	}
	
	return math_buffer;
}

/////////////////////////DETERMINANT

int arrange_pivot_det(Matrix *a, int col, int trackingpivot) {
	
	int pivotrow = find_pivot(a, col, trackingpivot);
	
	if(pivotrow >= a->rows) return 1;
	
	if(pivotrow != (trackingpivot + 1)) {
		swap_row(a, pivotrow, trackingpivot + 1); //still + 1 very inconvenient
		return 2; //if a swap happened, det function needs to know. go horse
	}

	return 0;
}

void eliminate_column_det(Matrix *a, int col, int row) {
	
	float val = a->data[row][col];
	float k = 0;
	
	for(int i = 0; i < a->rows; i++) {
		
		if(i == row || a->data[i][col] == 0) continue;
		
		k = (a->data[i][col] / val);
		scale_row(a, k, row);
		
		if (fabs(a->data[row][col] + a->data[i][col]) > EPSILON) {
			
			scale_row(a, -1, row);
			k *= -1;
		}
		
		add_row(a, i, row);
		scale_row(a, (1/k), row);
	}
}

//use upper triangular method

float det(Matrix a) {
	
	float determinant = 1;
	int trackingpivot = -1;
	int singular;
	
	if (a.rows != a.cols) {
		
		puts("Not a square matrix\n");
		return 0;
	}
	
	//using math_buffer for now
	math_buffer = createMatrix(a.rows, a.cols);
	
	for(int i = 0; i < a.rows; i++) {
		for(int k = 0; k < a.cols; k++) {
			
			math_buffer.data[i][k] = a.data[i][k];
		}
	}
	
	//just gonna first check if it's nonsingular, using altered rref function
	for(int i = 0; i < a.cols; i++) {
		
		if(arrange_pivot_det(&math_buffer, i, trackingpivot) == 1) {
			continue;
		}
		
		trackingpivot++;
		eliminate_column_det(&math_buffer, i, trackingpivot);
	}
	
	for(int i = 0; i < a.rows; i++) {
		reduce_row(&math_buffer, i);
	}
	
	singular = singularity_check(math_buffer);
	
	if(singular == 1) {
		
		puts("RREF:");
		printMatrix(math_buffer);
		
		puts("Matrix is singular\n");
		return 0;
	}
	
	//reset math_buffer and use altered eliminate column loop. go horse
	
	for(int i = 0; i < a.rows; i++) {
		for(int k = 0; k < a.cols; k++) {
			
			math_buffer.data[i][k] = a.data[i][k];
		}
	}
	
	for(int i = 0; i < a.rows; i++) {
		
		if (arrange_pivot_det(&math_buffer, i, i-1) == 2) {
			
			determinant *= -1;
		}
		
		eliminate_column_det(&math_buffer, i, i);
	}
	
	for(int i = 0; i < a.rows; i++) {
		
		determinant *= math_buffer.data[i][i];
	}
	
	return determinant;
}

/////////////////////////RANK

int get_rank(Matrix a) {
	
	int rank = 0;
	int trackingpivot = -1;
	
	math_buffer = createMatrix(a.rows, a.cols);
	
	for(int i = 0; i < a.rows; i++) {
		for(int k = 0; k < a.cols; k++) {
			
			math_buffer.data[i][k] = a.data[i][k];
		}
	}
	
	for(int i = 0; i < a.cols; i++) {
		
		if(arrange_pivot_det(&math_buffer, i, trackingpivot) == 1) {
			continue;
		}
		
		trackingpivot++;
		eliminate_column_det(&math_buffer, i, trackingpivot);
	}
	
	rank = trackingpivot + 1;
	
	return rank;
}

/////////////////////////INVERSE

//same as rref, just apply the same transformations to an identity matrix

int arrange_pivot_inv(Matrix *a, Matrix *b, int col, int trackingpivot) {
	
	int pivotrow = find_pivot(a, col, trackingpivot);
	
	if(pivotrow >= a->rows) return 1;
	
	if(pivotrow != (trackingpivot + 1)) {
		
		swap_row(a, pivotrow, trackingpivot + 1);
		swap_row(b, pivotrow, trackingpivot + 1);
	}

	return 0;
}

void eliminate_column_inv(Matrix *a, Matrix *b, int col, int row) {
	
	float val = a->data[row][col];
	float k = 0;
	
	for(int i = 0; i < a->rows; i++) {
		
		if(i == row || a->data[i][col] == 0) continue;
		
		k = (a->data[i][col] / val);
		
		scale_row(a, k, row);
		scale_row(b, k, row);
		
		if (fabs(a->data[row][col] + a->data[i][col]) > EPSILON) {
			
			scale_row(a, -1, row);
			scale_row(b, -1, row);
			k *= -1;
		}
		
		add_row(a, i, row);
		add_row(b, i, row);
		
		scale_row(a, (1/k), row);
		scale_row(b, (1/k), row);
	}
}

void reduce_row_inv(Matrix *a, Matrix *b, int row) {
	
	float k = 0;
	
	for(int i = 0; i < a->cols; i++) {
		
		if(fabs(a->data[row][i]) < EPSILON) continue;
		
		k = a->data[row][i];
		break;
	}
	
	if(fabs(k) > EPSILON) {
		scale_row(a, (1/k), row);
		scale_row(b, (1/k), row);
	}
}

Matrix inverse(Matrix a) {
	
	int trackingpivot = -1;
	
	math_buffer = createMatrix(a.rows, a.cols);
	
	Matrix math_buffer2 = createMatrix(a.rows, a.cols);
	
	for(int i = 0; i < a.rows; i++) {
		for(int k = 0; k < a.cols; k++) {
			
			math_buffer2.data[i][k] = a.data[i][k];
		}
	}
	
	for(int i = 0; i < a.rows; i++) {
		for(int k = 0; k < a.cols; k++) {
			
			if(i == k) {
				
				math_buffer.data[i][k] = 1;
				continue;
			}
			
			math_buffer.data[i][k] = 0;
		}
	}
	
	for(int i = 0; i < a.cols; i++) {
		
		if(arrange_pivot_inv(&math_buffer2, &math_buffer, i, trackingpivot) == 1) {
			continue;
		}
		
		trackingpivot++;
		eliminate_column_inv(&math_buffer2, &math_buffer, i, trackingpivot);
	}
	
	for(int i = 0; i < a.rows; i++) {
		reduce_row_inv(&math_buffer2, &math_buffer, i);
	}
	
	deleteMatrix(&math_buffer2);
	
	return math_buffer;
}
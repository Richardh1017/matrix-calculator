#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "calculator.h"

Matrix math_buffer = {0};

//////////////basic

Matrix add(Matrix a, Matrix b) {
	
	math_buffer = createMatrix(a.rows, a.cols);
	
	for(int i = 0; i < math_buffer.rows; i++) {
		
		for(int j = 0; j < math_buffer.cols; j++) {
			
			math_buffer.data[i][j] = a.data[i][j] + b.data[i][j];
			
			if(fabs(math_buffer.data[i][j]) < EPSILON) math_buffer.data[i][j] = 0;
		}
	}
	
	return math_buffer;
}

Matrix subtract(Matrix a, Matrix b) {
	
	math_buffer = createMatrix(a.rows, a.cols);
	
	for(int i = 0; i < math_buffer.rows; i++) {
		
		for(int j = 0; j < math_buffer.cols; j++) {
			
			math_buffer.data[i][j] = a.data[i][j] - b.data[i][j];
			
			if(fabs(math_buffer.data[i][j]) < EPSILON) math_buffer.data[i][j] = 0;
		}
	}
	
	return math_buffer;
}

Matrix kmult(Matrix a, float k) {
	
	math_buffer = createMatrix(a.rows, a.cols);
	
	for(int i = 0; i < math_buffer.rows; i++) {
		
		for(int j = 0; j < math_buffer.cols; j++) {
			
			math_buffer.data[i][j] = a.data[i][j] * k;
			
		}
		
	}
	
	return math_buffer;
}

Matrix multiply(Matrix a, Matrix b) {
	
	float temp = 0;
	
	math_buffer = createMatrix(a.rows, b.cols);
	
	for(int i = 0; i < a.rows; i++) {
		
		for(int j = 0; j <b.cols; j++) {
			
			for(int k = 0; k < a.cols; k++) {
				
				temp += a.data[i][k] * b.data[k][j];
				
			}
			
			math_buffer.data[i][j] = temp;
			temp = 0;
			
			if(fabs(math_buffer.data[i][j]) < EPSILON) math_buffer.data[i][j] = 0;
			
		}
	}
	
	return math_buffer;
}

Matrix transpose(Matrix a) {
	
	math_buffer = createMatrix(a.cols, a.rows);
	
	for(int i = 0; i < math_buffer.rows; i++) {
		
		for(int j = 0; j < math_buffer.cols; j++) {
			
			math_buffer.data[i][j] = a.data[j][i];
		}
	}
	
	return math_buffer;
	
}

void saveQuestion(Matrix result) {
	
	char buffer[100];
	
	printf("save? y/n: ");
	fgets(buffer, sizeof(buffer) - 1, stdin);
	
	if (buffer[0] == 'y'){
		
		registry = increaseReg(registry);
		
		registry.data[registry.count - 1] = createMatrix(result.rows, result.cols);
		
		for(int i = 0; i < math_buffer.rows; i++) {
		
			for(int j = 0; j < math_buffer.cols; j++) {
				
				registry.data[registry.count - 1].data[i][j] = math_buffer.data[i][j];
				
			}
		}
	}
}
#include <stdio.h>
#include <stdlib.h>
#include "calculator.h"

Registry increaseReg(Registry r) {
	
	r.count++;
	r.data = realloc(r.data, sizeof(Matrix) * r.count);
	
	return r;
}

Registry decreaseReg(Registry r, int pos) {
	
	for(int i = pos + 1; i < r.count; i++) {
		r.data[i - 1] = r.data[i];
	}
	
	r.count--;
	r.data = realloc(r.data, sizeof(Matrix) * r.count);
	
	return r;
}

Matrix createMatrix(int row, int col) {
	
	Matrix m;
	
	m.rows = row;
	m.cols = col;
	
	m.data = malloc(sizeof(float*) * m.rows);
	
	for (int i = 0; i < m.rows; i++) {
		m.data[i] = malloc(sizeof(float) * m.cols);
	}
	
	return m;
}

void deleteMatrix(Matrix *m) {
	
	for(int i = 0; i < m->rows; i++) {
		free(m->data[i]);
	}
	
	free(m->data);
	
}

void fillMatrix(Matrix *m) {
	
	char buffer[100];
	float val;
	
	puts("");
	
	for(int i = 0; i < m->rows; i++) {
		
		for(int j = 0; j < m->cols; j++) {
			
			printf("(%d, %d) val: ", i, j);
			
			fgets(buffer, sizeof(buffer) - 1, stdin);
			
			val = atof(buffer);
			
			
			
			m->data[i][j] = val;
			
		}
	}
	puts("");
}

void printMatrix(Matrix m) {
	
	for(int i = 0; i < m.rows; i++) {
		
		printf("|");
		
		for(int j = 0; j < m.cols; j++) {
			
			printf("   %.2f|", m.data[i][j]);
			
		}
		puts("");
	}
	puts("");
}
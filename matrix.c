#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
	int exit = 0;
	
	printf("\ninput 'cancel' to stop\n");
	
	for(int i = 0; i < m->rows; i++) {
		
		for(int j = 0; j < m->cols; j++) {
			
			printf("(%d, %d) val: ", i, j);
			
			if(exit == 1) {
				
				m->data[i][j] = 0;
				
				puts("0");
				continue;
			}
			
			fgets(buffer, sizeof(buffer) - 1, stdin);
			
			//set exit to 1 to zero out the matrix so u can manually delete later
			
			if ((strcmp(buffer, "cancel\n") == 0) && exit == 0) {
				
				exit = 1;
				
				m->data[i][j] = 0;
				continue;
			}
			
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
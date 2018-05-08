#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "datagen.h"
#include "logistic.h"


int main(int argc, char **argv) {
	
	
	double count = 0.0;
	
	double A[3][5] = {{1,2,1,0,1}, 
						{1,1,2,3,1},
						{0,1,0,2,0}};
	double** Aptr = malloc(5*sizeof(double*));
	for (int i = 0; i < 5; i++) {
		Aptr[i] = malloc(3*sizeof(double));
		for (int j = 0; j < 3; j++) {
			Aptr[i][j] = A[j][i];
			printf("%lf ", Aptr[i][j]);
		}
		printf("\n");
	}
	printf("\n");
/*
	double B[5][5] = {{1,2,-1,1,0}, {1,0.5,0,2,2}, {0.5,1,-1,-1,-1}, {1,3,-2,2,0}, {0.5,2,-1,0,-2}};
						
	double** Bptr = malloc(5*sizeof(double*));
	for (int i = 0; i < 5; i++) {
		Bptr[i] = malloc(5*sizeof(double));
		for (int j = 0; j < 5; j++) {
			Bptr[i][j] = B[i][j];
			printf("%lf ", B[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	*/
	double D[5] = {2,1.5,-.5,1,-1.5};
	double* Dptr = malloc(5*sizeof(double));
	for (int i = 0; i < 5; i++) {
		Dptr[i] = D[i];
		printf("%lf ", Dptr[i]);
	}
	printf("\n\n");
	
	
	// initialize a (k x n) array as X.T @ D
	// Fill in X.T multiplied by column corresponding entry in D
	double** lhs = malloc(3 * sizeof(double*));
	for (int j = 0; j < 3; j++) {
		lhs[j] = malloc(5 * sizeof(double));
		for (int i = 0; i < 5; i++) {
			lhs[j][i] = Aptr[i][j] * Dptr[i];
			printf("%lf ", lhs[j][i]);
		}
		printf("\n");
	}
	printf("\n");
	
	
	
	
	// 2d array for multiplication and inverse
	double** mult = malloc(3*sizeof(double*));
	//double** inv = malloc(3*sizeof(double*));
	for (int i = 0; i < 3; i++) {
		mult[i] = malloc(3*sizeof(double));
		//inv[i] = malloc(3*sizeof(double));
	}
	//printf("Built mult\n");
	
	
	arr2d_mult(lhs, Aptr, &mult, 3, 5, 3);
	
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			printf("%lf ", mult[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	 
	
	
	/*
	arr2d_inv(mult, &inv, 3);
	
	
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			printf("%lf ", inv[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	*/
	return 0;
}
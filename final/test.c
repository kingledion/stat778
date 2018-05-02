#include <stdio.h>
#include <stdlib.h>

void printer(double** data) {
	
}

void helperfunction(double*** data, int n, int c) {
	
	printf("\nIn helper function\n");
	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < c; j++) {
			printf("(%i %i %lf) ", i, j, (*data)[i][j]);
		}
		printf("\n");
	}
	
}

int main(int argc, char **argv) {
	
	int n = 4; //number of rows
	int c = 4; //number of columns
	
	double count = 0.0;
	
	double** data = malloc(n * sizeof(double*));
	
	for (int i = 0; i < n; i++) {
		double* row = malloc(c * sizeof(double));
		for (int j = 0; j < c; j++) {
			row[j] = count;
			count += 1.2;
		}
		data[i] = row;
	}
	
	printf("In main function\n");
	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < c; j++) {
			printf("(%i %i %lf) ", i, j, data[i][j]);
		}
		printf("\n");
	}
	
	helperfunction(&data, n, c);
	
	return 0;
}
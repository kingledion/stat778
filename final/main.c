#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "datagen.h"
#include "logistic.h"



	
int main(int argc, char **argv) {
	

	char* filename = "./pima.dat";
	
	int colsize = 9; // 8 columns of data and an adde, colsized intercept
	double **data;
	int *response;
	
	int arrlen = read_dataset(filename, &data, &response, colsize);
	
	standardize(&data, arrlen, colsize);
	
	//Seed random number genera18tor; 
	srand(time(NULL));
	int samplesize = 50;
	
	
	double** dsub = malloc(samplesize * sizeof(double*));
	int* rsub = malloc(samplesize * sizeof(double));
	
	double mu_0 = 0.05;
	double mu_1 = 0.1;
	
	double B0[9] = {0, 1, -1, 1, 0, 0, 0, 0, 0}; // Hardcoded
	
	//mislabeled_1(data, response, &dsub, &rsub, arrlen, samplesize, mu_0, mu_1);
	//mislabeled_2(data, response, &dsub, &rsub, arrlen, samplesize, mu_0, mu_1, B0, colsize);
	//mislabeled_3(data, response, &dsub, &rsub, arrlen, samplesize, mu_0, mu_1, colsize);
	//mislabeled_4(data, response, &dsub, &rsub, arrlen, samplesize, mu_0, mu_1, colsize);
	
	double* parameters = logisticgd(data, response, arrlen, colsize);
	

	printf("\nGradient Descent Parameters:\n");
	for (int j = 0; j < colsize; j++) {
		printf("%.3lf ", parameters[j]);
	}
	printf("\n\n");
	
	parameters = logisticnr(data, response, arrlen, colsize);
	
	printf("\nGradient Descent Parameters:\n");
	for (int j = 0; j < colsize; j++) {
		printf("%.3lf ", parameters[j]);
	}
	printf("\n");
	return 0;
}

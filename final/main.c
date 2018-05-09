#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "datagen.h"
#include "logistic.h"


void printWarning() {
	printf("Pass exactly two arguments\n");
	printf("Data Source = 'pima' for original dataset; 'b0' for author modified response variables\n");
	printf("Mislabel type = 0 for no mislabel; 1-4 for the four mislabel settings from\n ");
	printf("\t\t\t Hung et al, section 4.1\n\n");
}


	
int main(int argc, char **argv) {
	
	if (argc != 3) {
		printWarning();
		//printf("argc!=4\n");
		exit(0);
	}
	int PIMA = 1; // Boolean for data source
	int VALIDARGS = 1; // Boolean for if the function arguments are valid
	
	// Check argv[1]
	//printf("%s\n", argv[1]);
	if (!strcmp(argv[1], "b0")) {
		PIMA = 0;
		//printf("argv[1] = 'b0'\n");
	} else if (strcmp(argv[1], "pima")) {
		VALIDARGS = 0;
		//printf("argv[1] != 'pima'\n");
	}
	
	// Check argv[2]
	int labtype = (int) strtol(argv[2], NULL, 0);
	if (labtype < 0 || labtype > 4) {
		VALIDARGS = 0;
		//printf("bad labtype\n");
	}
	
	if (!VALIDARGS) {
		printWarning();
		exit(0);
	}
	
	
	

	char* filename = "./pima.dat";
	
	int colsize = 9; // 8 columns of data and an adde, colsized intercept hardcoded
	double **data;
	int *response;
	int samplesize = 500;
	
	int arrlen = read_dataset(filename, &data, &response, colsize);
	
	standardize(&data, arrlen, colsize); // always standardize...
	
	//Seed random number genera18tor; 
	srand(time(NULL));
	
	double B0[9] = {0, 1, -1, 1, 0, 0, 0, 0, 0}; // Hardcoded
	
	if (!PIMA) {
		printf("Generating author created responses: %i", PIMA);
		response = generateResponse(data, B0, arrlen, colsize);
	}
	
	
	double** dsub = malloc(samplesize * sizeof(double*));
	int* rsub = malloc(samplesize * sizeof(int));
	
	double mu_0 = 0.05;
	double mu_1 = 0.1;
	
	if (labtype == 0) {
		selectRand(data, response, &dsub, &rsub, arrlen, samplesize);
	}
	if (labtype == 1) {
		mislabeled_1(data, response, &dsub, &rsub, arrlen, samplesize, mu_0, mu_1);
	}
	else if (labtype == 2) {
		mislabeled_2(data, response, &dsub, &rsub, arrlen, samplesize, mu_0, mu_1, B0, colsize);
	}
	else if (labtype == 3) {
		mislabeled_3(data, response, &dsub, &rsub, arrlen, samplesize, mu_0, mu_1, colsize);
	}
	else if (labtype == 4) {
		mislabeled_4(data, response, &dsub, &rsub, arrlen, samplesize, mu_0, mu_1, colsize);
	}
	
	
	double* solution = logisticgd(dsub, rsub, samplesize, colsize);
	
	printf("Gradient Descent Parameters:\n");
	for (int j = 0; j < colsize; j++) {
		printf("%.3lf ", solution[j]);
	}
	printf("\n");
	
	solution = logisticnr(dsub, rsub, samplesize, colsize);
	
	printf("Newton Raphson Parameters:\n");
	for (int j = 0; j < colsize; j++) {
		printf("%.3lf ", solution[j]);
	}
	printf("\n");
	return 0;
}

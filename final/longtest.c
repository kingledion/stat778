#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "datagen.h"
#include "logistic.h"



void estGaussParams(double* sample, int arrlen, double* meanEst, double* varEst) {
	

	double* iterator = sample;
	
	// mean is sample mean
	double cumSum = 0;
	for (int i = 0; i < arrlen; i++, iterator++) {
		cumSum += *iterator;
	}
	double sampleMean = cumSum / arrlen;

	// use sample mean to calculate sample variance
	iterator = sample;
	cumSum = 0;
	for (int i = 0; i < arrlen; i++, iterator++) {
		cumSum += pow(*iterator - sampleMean, 2.0);
	}
	double sampleVar = cumSum / (arrlen - 1);  // use unbiased estimator
	
	*meanEst = sampleMean;
	*varEst = sampleVar;
}

int main(int argc, char **argv) {

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
	
	double** dsub = malloc(samplesize * sizeof(double*));
	int* rsub = malloc(samplesize * sizeof(int));
	
	double mu_0 = 0.05;
	double mu_1 = 0.1;
	
	double** parsums = malloc(colsize * sizeof(double*));
	for (int j = 0; j < colsize; j++) {
		parsums[j] = malloc(100 * sizeof(double*));
	}
	double par_mean = 0;
	double par_var = 0;
	double interval = 0;
	
	for (int i = 0; i < 100; i++) { // 100 iterations!
	
		selectRand(data, response, &dsub, &rsub, arrlen, samplesize);
		
		double* solution = logisticgd(dsub, rsub, samplesize, colsize);
		for (int j = 0; j < colsize; j++) {
			parsums[j][i] = solution[j];
		}
	}
	printf("No mislabel - Params\n");
	for (int j = 0; j < colsize; j++) {
		estGaussParams(parsums[j], 100, &par_mean, &par_var);
		interval = 1.6 * sqrt(par_var);
		printf("%lf %lf %lf\n", par_mean, par_mean - interval, par_mean + interval);

	}
	
	for (int i = 0; i < 100; i++) { // 100 iterations!
	
		mislabeled_1(data, response, &dsub, &rsub, arrlen, samplesize, mu_0, mu_1);
		
		double* solution = logisticgd(dsub, rsub, samplesize, colsize);
		for (int j = 0; j < colsize; j++) {
			parsums[j][i] = solution[j];
		}
	}
	printf("Mislabel 1 - Params\n");
	for (int j = 0; j < colsize; j++) {
		estGaussParams(parsums[j], 100, &par_mean, &par_var);
		interval = 1.6 * sqrt(par_var);
		printf("%lf %lf %lf\n", par_mean, par_mean - interval, par_mean + interval);

	}
	
	
	
	for (int i = 0; i < 100; i++) { // 100 iterations!
	
		mislabeled_2(data, response, &dsub, &rsub, arrlen, samplesize, mu_0, mu_1, B0, colsize);
		
		double* solution = logisticgd(dsub, rsub, samplesize, colsize);
		for (int j = 0; j < colsize; j++) {
			parsums[j][i] = solution[j];
		}
	}
	printf("Mislabel 2 - Params\n");
	for (int j = 0; j < colsize; j++) {
		estGaussParams(parsums[j], 100, &par_mean, &par_var);
		interval = 1.6 * sqrt(par_var);
		printf("%lf %lf %lf\n", par_mean, par_mean - interval, par_mean + interval);

	}
	
	
	for (int i = 0; i < 100; i++) { // 100 iterations!
	
		mislabeled_3(data, response, &dsub, &rsub, arrlen, samplesize, mu_0, mu_1, colsize);
		
		double* solution = logisticgd(dsub, rsub, samplesize, colsize);
		for (int j = 0; j < colsize; j++) {
			parsums[j][i] = solution[j];
		}
	}
	printf("Mislabel 3 - Params\n");
	for (int j = 0; j < colsize; j++) {
		estGaussParams(parsums[j], 100, &par_mean, &par_var);
		interval = 1.6 * sqrt(par_var);
		printf("%lf %lf %lf\n", par_mean, par_mean - interval, par_mean + interval);

	}
	
	
	for (int i = 0; i < 100; i++) { // 100 iterations!
	
		mislabeled_4(data, response, &dsub, &rsub, arrlen, samplesize, mu_0, mu_1, colsize);
		
		double* solution = logisticgd(dsub, rsub, samplesize, colsize);
		for (int j = 0; j < colsize; j++) {
			parsums[j][i] = solution[j];
		}
	}
	printf("Mislabel 4 - Params\n");
	for (int j = 0; j < colsize; j++) {
		estGaussParams(parsums[j], 100, &par_mean, &par_var);
		interval = 1.6 * sqrt(par_var);
		printf("%lf %lf %lf\n", par_mean, par_mean - interval, par_mean + interval);

	}
		
		
		
		
		
		
		
/*
	

	

	

	
*/
	
	

	

	
	return 0;
}

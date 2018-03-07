#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


void getRandNorm(double* randnorm, int n) 
{
	// Generate n random variables
	// Declare variables
	double u1; double u2;
	double z1; double z2;
	const double pi = 3.1415926535897;	
	int arrptr = 0;
	
	while (arrptr < n) { 
		// Generate two uniform random variables
		u1 = rand()/ (double) RAND_MAX;
		u2 = rand()/ (double) RAND_MAX;
		
		// Box-Muller transform
		randnorm[arrptr] = sqrt(-2*log(u1))*cos(2*pi*u2)*2.0-0.5;
		arrptr++;
		if (arrptr < n) { // for an odd n, we cannot add off the end
			randnorm[arrptr] = sqrt(-2*log(u2))*cos(2*pi*u1)*2.0-0.5;
			arrptr++;
		}
	}	
}

void estGaussParams(double* sample, int arrlen, double* meanEst, double* meanVar, double* varEst, double* varVar) {
	

	double* iterator = sample;
	
	// mean is sample mean
	double cumSum = 0;
	for (int i = 0; i < arrlen; i++, ++iterator) {
		cumSum += *sample;
		printf("%lf\n", cumSum);
	}
	double sampleMean = cumSum / arrlen;
	
	// use sample mean to calculate sample variance
	iterator = sample;
	cumSum = 0;
	for (int i = 0; i < arrlen; i++, ++iterator) {
		cumSum += pow(*sample - sampleMean, 2.0);
	}
	double sampleVar = cumSum / arrlen;
	
	meanEst = &sampleMean;
	varEst = &sampleVar;
	*meanVar = 0.0;
	*varVar = 0.0;
	
	
	
}


int main(int argc, char **argv)
{
	// Process arguments, store number of iterations in variable 'n'
	if (argc != 2) {
		printf("Pass only one argument; n = number of iterations\n");
		return 0;
	}
	int n = atoi(argv[1]);
	
	// Declare array to store random normal variables
	double *randnorm = (double*) malloc( sizeof(double)*n);
	
	//Seed random number generator; 
	srand(time(NULL));
	
	if (randnorm != NULL) {
		getRandNorm(randnorm, n);
	}
	else {
		printf("ERROR: No memory allocated to randnorm pointer!");
	}
	
	/*
	// Write to stdout
	for (int i = 0; i < n; i++) {
		printf("%lf\n", randnorm[i]);
	}*/
	
	// Declare variables for estimates
	double* meanEst; double* meanVar;
	double* varEst; double* varVar;
	
	estGaussParams(randnorm, n, meanEst, meanVar, varEst, varVar);
	
	printf("Estimated Mean: %lf\nMean variance: %lf\nEstimated Variance: %lf\nVariance variance: %lf\n", *meanEst, *meanVar, *varEst, *varVar);
	
	return 0;
}


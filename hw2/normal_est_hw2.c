#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <gsl/gsl_math.h>


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

void estGaussParams(double* sample, int arrlen, double* meanEst, double* varEst) {
	

	double* iterator = sample;
	
	// mean is sample mean
	double cumSum = 0;
	for (int i = 0; i < arrlen; i++, iterator++) {
		cumSum += *iterator;
	}
	double sampleMean = cumSum / (double) arrlen;

	
	// use sample mean to calculate sample variance
	iterator = sample;
	cumSum = 0;
	for (int i = 0; i < arrlen; i++, iterator++) {
		cumSum += pow(*iterator - sampleMean, 2.0);
	}
	double sampleVar = cumSum / (double) arrlen;
	
	*meanEst = sampleMean;
	*varEst = sampleVar;
	
	
	
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
	double meanEst = 0; 
	double varEst = 0; 
	
	estGaussParams(randnorm, n, &meanEst, &varEst);
	
	// I had to isntall libgsl myself to get gaussian and chi^2 distributions
	// so I will hard code instead of referencing package so this runs on any
	// machine
	
	double chiLo = gsl_cdf_chisq_P(0.975, n-1);
	printf("ChiLo: %lf\n", chiLo);
	
	// For 95% interval z = 1.960
	double meanLo = meanEst - 1.960*sqrt(varEst/n);
	double meanHi = meanEst + 1.960*sqrt(varEst/n);
	// Chi-square values for df= n-1 = 199 are 0.975=239.96; 0.025=161.826
	double varLo = varEst * (n-1) / 239.960;
	double varHi = varEst * (n-1) / 161.826;
	
	
	printf("Estimated Mean: (%lf, %lf)\n", meanLo, meanHi);
	printf("Estimated Variance: (%lf, %lf)\n", varLo, varHi);
	
	return 0;
}


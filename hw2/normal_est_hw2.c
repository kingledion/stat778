#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
//#include <gsl/gsl_math.h>


void getRandNorm(double mean, double var, double* randnorm, int n) 
{
	// Generate n random variables
	// Declare variables
	double u1; double u2;
	double z1; double z2;
	const double pi = 3.1415926535897;	
	int arrptr = 0;
	double sigma = sqrt(var); // the standard deviation
	
	while (arrptr < n) { 
		// Generate two uniform random variables
		u1 = rand()/ (double) RAND_MAX;
		u2 = rand()/ (double) RAND_MAX;
		
		// Box-Muller transform
		randnorm[arrptr] = sqrt(-2*log(u1))*cos(2*pi*u2)*sigma+mean;
		arrptr++;
		if (arrptr < n) { // for an odd n, we cannot add off the end
			randnorm[arrptr] = sqrt(-2*log(u2))*cos(2*pi*u1)*sigma+mean;
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

// Sums a vector, used on boolean vectors to calculate coverage probability
double vectorsum (int* vector, int len) {
	double runningsum = 0;
	for (int i = 0; i < len; i++) {
		runningsum += vector[i];
	}
	return runningsum;
}


int main(int argc, char **argv)
{
	// Process arguments, store number of random normals to generate in variable 'n'
	// store iterations in variable 'iter'
	if (argc != 5) {
		printf("Pass exactly four arguments:\n\ttruemean = true mean\n\truevar = true variance\n");
		printf("\t n = number random normals\n\titer = number of iterations\n");
		return 0;
	}
	double truemean = strtod(argv[1], NULL);
	double truevar = strtod(argv[2], NULL);
	int n = atoi(argv[3]);
	int iter = atoi(argv[4]); // No error checking for a bad argument being passed
							  // Should check for not negative and other factors
	
	//Seed random number generator; 
	srand(time(NULL));
	
	
	// I had to install libgsl myself to get gaussian and chi^2 distributions
	// so I will hard code the distribution values instead of referencing a 
	// package; that way this code will run on any machine with gcc. 
	
	/*
	double chiLo = gsl_cdf_chisq_P(0.975, n-1);
	printf("ChiLo: %lf\n", chiLo);
	*/
	
	// z-score for 95% confidence interval
	const double z95 = 1.960;
	// chi^2 scores for alpha = .975 and 0.025 at 49, 99, 199 df
	double chi2_975 = 0;
	double chi2_025 = 0;
	if (n == 50) {
		chi2_975 = 31.555;
		chi2_025 = 70.222;
	}
	else if (n == 100) {
		chi2_975 = 73.361;
		chi2_025 = 128.422;
	}
	else if (n == 200) {
		chi2_975 = 161.826;
		chi2_025 = 239.960;	
	}
	else {
		printf("Only 50, 100, and 200 are viable for n = number of random normals");
		return 0;
	}
	
	// Vectors of estimated means and variances; vector of booleans for coverage probability
	double allmeans[iter];
	double allvars[iter];
	int coveredMean[iter];
	int coveredVar[iter];
	
	// Declare array to store random normal variables
	double *randnorm = (double*) malloc( sizeof(double)*n);
		
	for (int i = 0; i < iter; i++) {
		
		if (randnorm != NULL) {
			getRandNorm(truemean, truevar, randnorm, n);
		}
		else {
			printf("ERROR: No memory allocated to randnorm pointer!");
		}
	
	
		// Declare variables for estimates
		double meanEst = 0; 
		double varEst = 0; 
		
		// Estimate mean and variance
		estGaussParams(randnorm, n, &meanEst, &varEst);
		
		// determine if true mean is covered; mean confidence interval using 95% z-score
		// store the boolean value directly into the array
		coveredMean[i] = (meanEst - z95*sqrt(varEst/n) < truemean) && (meanEst + z95*sqrt(varEst/n) > truemean);			
		
		// determine if true stdev is covered; variance confidence interval using chi^2 distribution
		coveredVar[i] = (varEst * (n-1) / chi2_025 < truevar) && (varEst * (n-1) / chi2_975 > truevar);
		
		// Store mean and variance estimates
		allmeans[i] = meanEst;
		allvars[i] = varEst;
			
	}
	
	/*
	for (int k = 0; k < iter; k++) {
		printf("%lf\n", allvars[k]);
	}*/
			
	// Calculate sample average and sample standard deviation of means
	double meanSampleMean = 0;
	double meanSampleVar = 0;
	estGaussParams(allmeans, iter, &meanSampleMean, &meanSampleVar);
	
	// Calculate sample average and sample standard deviation of varainces
	double varSampleMean = 0;
	double varSampleVar = 0;
	estGaussParams(allvars, iter, &varSampleMean, &varSampleVar);
	
	
	

	// Calculate the sum of the sample mean standard errors
	double sumMeanStdError = 0; 
	for (int j = 0; j < iter; j++) {
		sumMeanStdError += sqrt(allvars[j]/n); // allvars is variance, so sqrt(allvars) = stdev
	}
	
	// Calculate the sum of the sample variance standard errors
	double sumVarStdError = 0; 
	for (int j = 0; j < iter; j++) {
		sumVarStdError += allvars[j]*sqrt(2.0/(n-1)); 
	}
	
	printf("Sample avg of mean estimates: %lf\n", meanSampleMean);
	printf("Sample stdev of mean estimates: %lf\n", sqrt(meanSampleVar));
	printf("Sample avg of std error of means: %lf\n", sumMeanStdError / iter);
	printf("Mean coverage probability: %lf\n", vectorsum(coveredMean, iter)/ iter);
	printf("\n");
	printf("Sample avg of variance estimates: %lf\n", varSampleMean);
	printf("Sample stdev of variance estimates: %lf\n", sqrt(varSampleVar));
	printf("Sample avg of std error of variances: %lf\n", sumVarStdError / iter);
	printf("Variance coverege probability: %lf\n", vectorsum(coveredVar, iter)/ iter);
	
	return 0;
}


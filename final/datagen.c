#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "datagen.h"



// Random normal generator from hw2
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

void selectRand(double** data, int* response, double*** dsub, int** rsub, int arrlen, int samplesize) {
	if (samplesize >= arrlen) {
		printf("Sample size is larger than data array\n");
		exit(1);
	}
	
	int i, n;
	int numsleft = arrlen;
	
	int selections[arrlen];
	for (i = 0; i < arrlen; i++) {
		selections[i] = i;
	}
	
	for (i = 0; i < samplesize; i++) {
		n = rand() % numsleft;
		(*dsub)[i] = data[selections[n]];
		(*rsub)[i] = response[selections[n]];
		selections[n] = selections[numsleft - 1];
		numsleft--;
	}

} 

// Mislabels Y data under possibility S1
void mislabeled_1(double** data, int* response, double*** dsub, int** rsub, int arrlen, int samplesize, double mu0, double mu1) {
	
	selectRand(data, response, dsub, rsub, arrlen, samplesize);
	
	double rnd;
	
	double eta0 = mu0;
	double eta1 = mu1;
	
	for (int i = 0; i < samplesize; i++) {
		rnd = rand()/ (double) RAND_MAX;

		if ((*rsub)[i] == 0) {  // eta_0 mislabel error
			if (rnd < eta0) {
				(*rsub)[i] = 1;
			}
		}
		else { // rsub[i] == 0; eta_1 mislabel error
			if (rnd < eta1) {
				(*rsub)[i] = 0;
			}
		}
	}
	
}

// Mislabels Y data under possibility S2
void mislabeled_2(double** data, int* response, double*** dsub, int** rsub, int arrlen, int samplesize, double mu0, double mu1, double* B0, int c) {
	selectRand(data, response, dsub, rsub, arrlen, samplesize);
	
	double rnd;
	
	double eta;
	
	for (int i = 0; i < samplesize; i++) {
		
		//printf("%i %i ", i, (*rsub)[i]);
		
		rnd = rand()/ (double) RAND_MAX;
		double B0x = 0;
		for (int j = 0; j < c; j++) {
			B0x += B0[j] * (*dsub)[i][j];
		}
		eta = mu0 + (mu1 - mu0) * exp(B0x) / (1 + exp(B0x));

		if ((*rsub)[i] == 0) {  // eta_0 mislabel error
			if (rnd < eta) {
				(*rsub)[i] = 1;
			}
		}
		else { // rsub[i] == 0; eta_1 mislabel error
			if (rnd < eta) {
				(*rsub)[i] = 0;
			}
		}
		//printf("%i\n", (*rsub)[i]);
	}
}

// Mislabels Y data under possibility S3
void mislabeled_3(double** data, int* response, double*** dsub, int** rsub, int arrlen, int samplesize, double mu0, double mu1, int c) {
	selectRand(data, response, dsub, rsub, arrlen, samplesize);
	
	double rnd;
	
	// Get 18 random normal variables
	double* randnorms = malloc(c * 2 * sizeof(double));
	getRandNorm(0.0, 4.0, randnorms, c * 2);
	
	double* b0 = malloc(c * sizeof(double));
	double* b1 = malloc(c * sizeof(double));
	
	for (int j = 0;	 j < c; j++) {
		b0[j] = randnorms[j];
		b1[j] = randnorms[j+c];
	}
	
	double eta;
	
	for (int i = 0; i < samplesize; i++) {
		
		//printf("%i %i ", i, (*rsub)[i]);
		
		rnd = rand()/ (double) RAND_MAX;

		if ((*rsub)[i] == 0) {  // eta_0 mislabel error
			double bjx = 0;
			for (int j = 0; j < c; j++) {
				bjx += b0[j] * (*dsub)[i][j];
			}
			eta = mu0 + (mu1 - mu0) * exp(bjx) / (1 + exp(bjx));
			
			if (rnd < eta) {
				(*rsub)[i] = 1;
			}
		}
		else { // rsub[i] == 0; eta_1 mislabel error
			double bjx = 0;
			for (int j = 0; j < c; j++) {
				bjx += b1[j] * (*dsub)[i][j];
			}
			
			eta = mu0 + (mu1 - mu0) * exp(bjx) / (1 + exp(bjx));
			if (rnd < eta) {
				(*rsub)[i] = 0;
			}
		}
		//printf("%i\n", (*rsub)[i]);
	}
	
}

// Mislabels Y data under possibility S4
void mislabeled_4(double** data, int* response, double*** dsub, int** rsub, int arrlen, int samplesize, double mu0, double mu1, int c) {
	selectRand(data, response, dsub, rsub, arrlen, samplesize);
	
	double rnd;
	
	// Get 18 random normal variables
	double* randnorms = malloc(2 * sizeof(double)); // Its going to make two rand norms anyways...
	getRandNorm(2.0, 0.09, randnorms, 2);
	
	double a = randnorms[0];
	
	double eta;
	
	for (int i = 0; i < samplesize; i++) {
		
		//printf("%i %i ", i, (*rsub)[i]);
		
		rnd = rand()/ (double) RAND_MAX;

		if ((*rsub)[i] == 0) {  // eta_0 mislabel error
			eta = mu0;
			if (abs(data[i][1] - a) < 3 && abs(data[i][3] + a) < 3) {
				eta = mu1;
			}			
			if (rnd < eta) {
				(*rsub)[i] = 1;
			}
		}
		else { // rsub[i] == 0; eta_1 mislabel error
			eta = mu0;
			if (abs(data[i][1] + a) < 3 && abs(data[i][2] + a) < 3) {
				eta = mu1;
			}
			if (rnd < eta) {
				(*rsub)[i] = 0;
			}
		}
		//printf("%i\n", (*rsub)[i]);
	}
		
}

void standardize(double*** data, int n, int k) { // n rows and c columns

	// Note all j loops start at 1: first colum, colsizen is intercept, do not standardize

	double* sums = malloc(k * sizeof(double)); // assume all doubles
	double* sumsq = malloc(k * sizeof(double)); 
	
	// Initialize sums to zero
	for (int j = 1; j < k; j++) {
		sums[j] = 0.0;
		sumsq[j] = 0.0;
	}
	
	// Sum all columns
	for (int i = 0; i < n; i++) {
		for (int j = 1; j < k; j++) {
			//printf("%i %	printf("Finished here\n");i %lf\n", i, j, (*data)[i][j]);
			sums[j] += (*data)[i][j];
		}
	}
	// Mean of all columns
	for (int j = 1; j < k; j++) {
		sums[j] = sums[j] / n;
	}

	// Get sum of squares for variance
	for (int i = 0; i < n; i++) {
		for (int j = 1; j < k; j++) {
			sumsq[j] += pow((*data)[i][j] - sums[j], 2.0);
		}
	}
	// Std Dev of all columns
	for (int j = 1; j < k; j++) {
		sumsq[j] = pow(sumsq[j] / (n - 1), 0.5); // use unbiased estimator
	}


	for (int i = 0; i < n; i++) {
		for ( int j = 1; j < k; j++) {
			(*data)[i][j] = ((*data)[i][j] - sums[j])/sumsq[j];
		}
	}
	
}


// Built in assumption is that the first column in the dataset is the response data
// Automatically adds intercept as first column of read data

int read_dataset(char* filename, double*** d, int** r, int colsize) {
	

	
	// Open input file
	FILE *fpin;
	if( (fpin = fopen(filename, "r")) == NULL) {
		printf("File not found\n");
		exit(1);	
	}
	
	int arrlen = 0;   // This is the next element to write	for both arrays
	int j = 0;
	
	int n = 100; // Start with size 100 array for reading data; expand as needed
	
	//  Allocate a dynamic input size
	double** data = malloc(n * sizeof(double*));
	int* response = malloc(n * sizeof(int));
	
	// Throw away the first line of the input file
	fscanf(fpin, "%*[^\n]\n");
	
	// Read input to struct array
    int resp; int x1in; int x2in; int x3in; int x4in; int x5in; double x6in; double x7in; int x8in;
	double x1d; double x2d; double x3d; double x4d; double x5d; double x8d; 
	while (fscanf(fpin, "%i %i %i %i %i %i %lf %lf %i", &resp, &x1in, &x2in, &x3in, &x4in, &x5in, &x6in, &x7in, &x8in) == colsize) {
		
		// if the allocated space is too small, make it bigger
		if (arrlen >= n - 1) {
			
			data = realloc(data, n * 2 * sizeof(double*));
			response = realloc(response, n * 2 * sizeof(int));
			if (data == NULL | response == NULL) {
				printf("Cannot allocate additional memory");
				exit(1);
			}
			n = n * 2;
		}
		
		// cast ints to doubles//for (int i)
		x1d = (double) x1in;
		x2d = (double) x2in;
		x3d = (double) x3in;
		x4d = (double) x4in;
		x5d = (double) x5in;
		x8d = (double) x8in;
		
		double *t = malloc(9 * sizeof(double));
		t[0] = 1.0; t[1] = x1d; t[2] = x2d; t[3] = x3d; t[4] = x4d; 
		t[5] = x5d; t[6] = x6in; t[7] = x7in; t[8] = x8d;
		data[arrlen] = t;
		response[arrlen] = resp;
		arrlen++;
		
	}
	fclose(fpin);
	
	*d = data;
	*r = response;
	
	return arrlen;
}

// Generates logit response for a given set of parameters
int* generateResponse(double** data, double* params, int n, int k) {
	
	int* response = malloc(n * sizeof(double));
	double xb = 0;
	
	for (int i = 0; i < n; i++) {
		xb = 0;
		for (int j = 0; j < k; j++) {
			xb += data[i][j] * params[j];
		}
		response[i] = rint(1/(1+exp(-1*xb)));
	}
	
	return response;
	
	
	
}
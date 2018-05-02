#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


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

void selectRand(double** data, int* response, double** dsub, int* rsub, int arrlen, int samplesize) {
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
		dsub[i] = data[selections[n]];
		rsub[i] = response[selections[n]];
		selections[n] = selections[numsleft - 1];
		numsleft--;
	}

} 

// Mislabels Y data under possibility S1
void mislabeled_1(double** data, int* response, double*** dsub, int** rsub, int arrlen, int samplesize, double mu_0, double mu_1) {
	
	selectRand(data, response, *dsub, *rsub, arrlen, samplesize);
	
	double rnd;
	
	double eta0 = mu_0;
	double eta1 = mu_1;
	
	for (int i = 0; i < samplesize; i++) {
		rnd = rand()/ (double) RAND_MAX;

		if ((*rsub)[i] == 0) {  // eta_0 mislabel error
			if (rnd < eta_0) {
				(*rsub)[i] = 1;
			}
		}
		else { // rsub[i] == 0; eta_1 mislabel error
			if (rnd < eta_1) {
				(*rsub)[i] = 0;
			}
		}
	}
	
}

// Mislabels Y data under possibility S2
// NEEDS MUCH MORE WORK!!!!!!!!
void mislabeled_2(double** data, int* response, double*** dsub, int** rsub, int arrlen, int samplesize, double mu_0, double mu_1) {
	selectRand(data, response, *dsub, *rsub, arrlen, samplesize);	
}

// Mislabels Y data under possibility S3
void mislabeled_3(double** data, int* response, double*** dsub, int** rsub, int arrlen, int samplesize, double mu_0, double mu_1, int c) {
	selectRand(data, response, *dsub, *rsub, arrlen, samplesize);
	
	double rnd;
	
	// Get 18 random normal variables
	double* randnorms = malloc(18 * sizeof(double));
	getRandNorm(0.0, 4.0, randnorms, 18);
	
	double b1 = malloc(c * sizeof(double));
	double b2 = malloc(c * sizeof(double));
	
	for (int j = 0; j < c; j++) {
		b1[j] = randnorms[j];
		b2[j] = randnorms[j+c];
	}
	
	for (int i = 0; i < samplesize; i++) {
		
		printf("%i %i ", i, (*rsub)[i])
		
		rnd = rand()/ (double) RAND_MAX;
		double exp_bjx = 0;
		for (int j = 0; j < c; j++) {
			
		}

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
		printf("%i\n", (*rsub)[i])
	}
	
}

// Mislabels Y data under possibility S4
void mislabeled_4(double** data, int* response, double*** dsub, int** rsub, int arrlen, int samplesize, double mu_1) {
	selectRand(data, response, *dsub, *rsub, arrlen, samplesize);
	
	double mu_0 = 0.05;   // As specified in the paper
	double rnd;
	
	for (int i = 0; i < samplesize; i++) {
		rnd = rand()/ (double) RAND_MAX;

		if ((*rsub)[i] == 0) {  // eta_0 mislabel error
			if (rnd < mu_0) {
				(*rsub)[i] = 1;
			}
		}
		else { // rsub[i] == 0; eta_1 mislabel error
			if (rnd < mu_1) {
				(*rsub)[i] = 0;
			}
		}
	}
		
}

void standardize(double*** data, int n, int c) { // n rows and c columns

	// Note all j loops start at 1: first column is intercept, do not standardize

	double* sums = malloc(c * sizeof(double)); // assume all doubles
	double* sumsq = malloc(c * sizeof(double)); 
	
	// Initialize sums to zero
	for (int j = 1; j < c; j++) {
		sums[j] = 0.0;
		sumsq[j] = 0.0;
	}
	
	// Sum all columns
	for (int i = 0; i < n; i++) {
		for (int j = 1; j < c; j++) {
			//printf("%i %	printf("Finished here\n");i %lf\n", i, j, (*data)[i][j]);
			sums[j] += (*data)[i][j];
		}
	}
	// Mean of all columns
	for (int j = 1; j < c; j++) {
		sums[j] = sums[j] / n;
	}

	// Get sum of squares for variance
	for (int i = 0; i < n; i++) {
		for (int j = 1; j < c; j++) {
			sumsq[j] += pow((*data)[i][j] - sums[j], 2.0);
		}
	}
	// Std Dev of all columns
	for (int j = 1; j < c; j++) {
		sumsq[j] = pow(sumsq[j] / (n - 1), 0.5); // use unbiased estimator
	}


	for (int i = 0; i < n; i++) {
		for ( int j = 1; j < c; j++) {
			(*data)[i][j] = ((*data)[i][j] - sums[j])/sumsq[j];
		}
	}
	
}


// Built in assumption is that the first column in the dataset is the response data
// Only works on pima dataset; 

int read_dataset(char* filename, double*** d, int** r) {
	

	
	// Open input file
	FILE *fpin;
	if( (fpin = fopen(filename, "r")) == NULL) {
		printf("File not found\n");
		exit(1);	
	}
	
	int arrlen = 0;   // This is the next element to write	for both arrays
	int j = 0;
	
	
	//  Allocate a dynamic input size
	double** data = malloc(100 * sizeof(double*));
	int* response = malloc(100 * sizeof(int));
	
	// Throw away the first line of the input file
	fscanf(fpin, "%*[^\n]\n");
	
	// Read input to struct array
    int resp; int x1in; int x2in; int x3in; int x4in; int x5in; double x6in; double x7in; int x8in;
	double x1d; double x2d; double x3d; double x4d; double x5d; double x8d; 
	while (fscanf(fpin, "%i %i %i %i %i %i %lf %lf %i", &resp, &x1in, &x2in, &x3in, &x4in, &x5in, &x6in, &x7in, &x8in) == 9) {
		
		// if the allocated space is too small, make it bigger
		if (arrlen >= n -1) {
			
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
	
int main(int argc, char **argv)
{
	

	char* filename = "./pima.dat";
	
	int colsize = 9;
	double **data;
	int *response;
	
	int arrlen = read_dataset(filename, &data, &response, colsize);
	
	standardize(&data, arrlen, colsize);
	
	//Seed random number generator; 
	srand(time(NULL));
	int samplesize = 50;
	
	
	double** dsub = malloc(samplesize * sizeof(double*));
	int* rsub = malloc(samplesize * sizeof(double));
	
	double mu_0 = 0.05;
	double mu_1 = 0.1;
	
	mislabeled_1(data, response, &dsub, &rsub, arrlen, samplesize, mu_0, mu_1);
	

	/*
	for (int i = 0; i < samplesize; i++) {
		printf("%i %i ", i, rsub[i]);
		for (int j = 0; j < 9; j++) {
			printf("%lf ", dsub[i][j]);
		}
		printf("\n");
	}*/
	return 0;
}

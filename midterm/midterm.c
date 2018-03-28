#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

const double pi = 3.14159265358979323846;




void getRandNorm(double* randnorm, double mean, double var, int n) 
{
	// Generate n random variables
	// Declare variables
	double u1; double u2;
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

void getRandNormCont(double* randNormCont, double mean, double var, int n) 
{
	// Get an array of random normal variables of length n
	getRandNorm(randNormCont, mean, var, n);
	
	double randUniform;
	
	// Iterate trhough the random normal array. With a 2% chance,
	// replace each random normal with a random uniform variable
	// 3 to 5 std deviations above the mean.
	for (int i = 0; i < n; i++) {
		randUniform = rand()/ (double) RAND_MAX;
		if (randUniform < 0.02) {
			// Transform to a random number between 3 and 5 stdevs above mean
			randNormCont[i] = randUniform * 250 * sqrt(var) + mean + sqrt(var) * 5;
			
		}
	}
	
}


void getRandExp(double* randExp, double mean, int n) {
	double u = 0;
	int arrptr = 0;
	
	while (arrptr < n) {
		u = rand()/ (double) RAND_MAX;
		
		// Inversion transform sampling
		randExp[arrptr] = log(1 - u) * (-1 * mean);
		arrptr++;
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

double ttest(double* sample1, double* sample2, int n) {
	
	double sample1Mean = 0;
	double sample1Var = 0;
	double sample2Mean = 0;
	double sample2Var = 0;
	
	estGaussParams(sample1, n, &sample1Mean, &sample1Var);
	estGaussParams(sample2, n, &sample2Mean, &sample2Var);
	
	// T-statistic. Assume different variances, and that n_1 = n_2
	return (sample1Mean - sample2Mean)/ sqrt(sample1Var/n + sample2Var/n);
	
}


// Structure to store tuples of data
struct tuple {double val; int source; };

// Compare function for array sorting; lowest number in lowest index
int comp (const void * a, const void * b) 
{
	
	const struct tuple *elem1 = a;    
    const struct tuple *elem2 = b;

	if (elem1->val > elem2->val)
		return 1;
	else if (elem1->val < elem2->val)
		return -1;
	return 0;
}

double mannwhitney(double* sample1, double* sample2, int n) {
	
	struct tuple data[n*2];
	int i = 0;
	
	for (i = 0; i < n; i++) {
        data[2*i] = (struct tuple) {sample1[i], 1};
        data[2*i+1] = (struct tuple) {sample2[i], 2};	
	}
	
	/*
	printf("Built List\n");
	for (i = 0; i < n*2; i++) {
		printf("%lf %i\n", data[i].val, data[i].source);
	}*/
	
	// sort in place is fine since we won't be using these again
	long unsigned int tlen = sizeof(struct tuple);
	qsort(data, n*2, tlen, comp);
	
	/*
	//printf("\nSorted List\n");
	for (i = 0; i < n*2; i++) {
		printf("%lf %i\n", data[i].val, data[i].source);
	}*/
	
	
	double ranksum1;
	int j; i = 0;
	// Loop through all structs
	//printf("\nMann-Whitney\n");
	while (i < n*2) {
		// Find all identical time values
		j = i + 1;
		//printf("%lf %i\n", data[i].val, data[i].source);
		while (j < n && data[j].val == data[i].val) {
			//printf("Identical: %lf %i\n", data[j].val, data[j].source);
			j++;
		} 
		
		// Average rank between i and j-1; if i=j-1; then rank = i+1
		double rank = (i + j - 1)/2+1; 
		// Iterate through all tied values, assign average rank to appropriate ranksum bins
		for (int k = i; k < j; k++) {
			if (data[k].source == 1) {
				ranksum1 += rank;
				//printf(">>>%lf %lf\n", rank, ranksum1);
			} // Only need to add ranks on one side; the total sum of ranks is known from n
		}
		
		// increment i to j
		i = j;
		
	}
	
	// U_1 = R_1 - n(n+1)/2 <---- the return value is the Mann-Whitney U statistic
	return ranksum1 - n*(n+1)/2;	
}

void inputErr() {
	printf("Pass exactly six arguments:\n\tdist = 'norm' for Normal, 'normc' for Normal with contamination (sample 2 is contaminated),\n");
	printf("\t\t'exp' for exponential\n");
	printf("\ttruemean1 = true mean for sample 1\n\ttruevar1 = true variance for sample 1\n");
	printf("\ttruemean2 = true mean for sample 2\n\ttruevar2 = true variance for sample 2 (ignored for exponential)\n");
	printf("\tn = number of random variables in each sample (same size in each group)\n");	
}

int main(int argc, char **argv)
{
	
	// Process arguments; explanation in text of inputErr() function
	if (argc != 7) {
		inputErr();
		return 0;
	}
	char* dist = argv[1];
	double truemean1 = strtod(argv[2], NULL);
	double truevar1 = strtod(argv[3], NULL);
	double truemean2 = strtod(argv[4], NULL);
	double truevar2 = strtod(argv[5], NULL);
	int n = atoi(argv[6]);
	

	
	// if input string is not in the set('norm', 'normc', 'exp') return an error
	char strnorm[] = "norm";
	char strnormc[] = "normc";
	char strexp[] = "exp";
	if (strcmp(dist, strnorm) && strcmp(dist, strnormc) && strcmp(dist, strexp)) {
		inputErr();
		return 0;
	}
	
	
	// Set critical values for two sided t-test based on n = 25, 50 or 100
	// t_95 is two tailed t at 0.975, etc; df = 2*n - 2
	// if n isn't in the set (25, 50, 100) then return an error
	double t_90 = 0.0; double t_95 = 0.0; double t_99 = 0.0;
	if (n == 25) {
		t_90 = 1.677224;
		t_95 = 2.010635;
		t_99 = 2.682204;
	}
	else if (n == 50) {
		t_90 = 1.660551;
		t_95 = 1.984467;
		t_99 = 2.626931;
	}
	else if (n == 100) {
		t_90 = 1.652586;
		t_95 = 1.972017;
		t_99 = 2.600887;
	}
	else {
		printf("Only 25, 50, and 100 are viable for n = sample size\n");
		return 0;
	}
	
	// Gerneate p-value for Mann-Whitney U test based on normal distribution
	// mu = n^2/2; sigma = sqrt(n^2*(2n+1)/12)
	// Use mu and sigma based on sample size to adjust to standard normal, two sided distribution
	double mu; double sigma;
	double z_90 = 1.644854; double z_95 = 1.959940; double z_99 = 2.575829;
	if (n == 25) {
		mu = 312.5; sigma = 51.538820;
	}
	else if (n == 50) {
		mu = 1250; sigma = 145.057460;
	}
	else if (n == 100) {
		mu = 5000; sigma = 409.267639;
	} // n not in (25, 50, 100) case should not be possible due to error checking t-test block
	
	
	int iter = 10000; // always use 1000 iterations
	int i = 0;
	
	//Seed random number generator; 
	srand(time(NULL));	
	
	// Declare array to store random variables
	double *sample1 = (double*) malloc( sizeof(double)*n);
	double *sample2 = (double*) malloc( sizeof(double)*n);
	
	if (sample1 == NULL || sample2 == NULL) {
		printf("ERROR: No memory allocated to sample pointer!");
		return 0;
	}
	
	// Sum null rejections for t-test
	double tscore = 0;
	int trejnull90 = 0;
	int trejnull95 = 0;
	int trejnull99 = 0;
	
	// Sum null rejections for Mann-Whitney U test
	double uscore = 0;
	int urejnull90 = 0;
	int urejnull95 = 0;
	int urejnull99 = 0;

	for (i = 0; i < iter; i++) {
	
		if (!strcmp(dist, strnorm)) {
			getRandNorm(sample1, truemean1, truevar1, n);
			getRandNorm(sample2, truemean2, truevar2, n);
		}
		else if (!strcmp(dist, strnormc)) { 
			getRandNorm(sample1, truemean1, truevar1, n);
			getRandNormCont(sample2, truemean2, truevar2, n);
		}
		else if (!strcmp(dist, strexp)) { 
			getRandExp(sample1, truemean1, n);
			getRandExp(sample2, truemean2, n);
		}
		
		tscore = fabs(ttest(sample1, sample2, n));
		uscore = mannwhitney(sample1, sample2, n);
		//printf("U-score: %lf\n", uscore);
		
		uscore = fabs(uscore - mu)/sigma;
		//printf("%lf %lf\n%lf\n", mu, sigma, uscore);
		//printf("U-score: %lf\n", uscore);
		

		
		if (tscore > t_90) {
			trejnull90++;
			if (tscore > t_95) {
				trejnull95++;
				if (tscore > t_99) {
					trejnull99++;
				}
			}
		}
		
		if (uscore > z_90) {
			urejnull90++;
			if (uscore > z_95) {
				urejnull95++;
				if (uscore > z_99) {
					urejnull99++;
				}
			}
		}
	}
	
	
	/*
	printf("Sample 1\n");
	for (i = 0; i < n; i++) {
		printf("%lf\n", sample1[i]);
	}
	printf("\nSample 2\n");
	for (i = 0; i < n; i++) {
		printf("%lf\n", sample2[i]);				
	}*/
	
	
	printf("\nT-test\n");
	
	if (truemean1 == truemean2 && (!strcmp(dist, strnorm) || !strcmp(dist, strexp))) {
		// Distributions are the same! Rejecting null is Type 1 error for t-test
		printf("Type 1 error for a = 0.90: %i/%i\n", trejnull90, iter);
		printf("Type 1 error for a = 0.95: %i/%i\n", trejnull95, iter);
		printf("Type 1 error for a = 0.99: %i/%i\n", trejnull99, iter);

	}
	else  { 
		//Distributions are not the same! Rejecting null contributes to power of test
		printf("Power for a = 0.90: %i/%i\n", trejnull90, iter);
		printf("Power for a = 0.95: %i/%i\n", trejnull95, iter);
		printf("Power for a = 0.99: %i/%i\n", trejnull99, iter);
	}
	
		printf("\nMann-Whitney U test\n");
	
	if (truemean1 == truemean2 && (!strcmp(dist, strexp) || (!strcmp(dist, strnorm) && truevar1 == truevar2))) {
		// Distributions are the same! Rejecting null is Type 1 error for t-test
		printf("Type 1 error for a = 0.90: %i/%i\n", urejnull90, iter);
		printf("Type 1 error for a = 0.95: %i/%i\n", urejnull95, iter);
		printf("Type 1 error for a = 0.99: %i/%i\n", urejnull99, iter);

	}
	else  { 
		//Distributions are not the same! Rejecting null contributes to power of test
		printf("Power for a = 0.90: %i/%i\n", urejnull90, iter);
		printf("Power for a = 0.95: %i/%i\n", urejnull95, iter);
		printf("Power for a = 0.99: %i/%i\n", urejnull99, iter);
	}
	
	return 0;
}

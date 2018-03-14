#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Structure to store tuples of data
struct tuple {
    double timeval; 
    int notcensored;
	double x1;
	int x2;
	
};

// Compare function for array sorting; lowest number in lowest index
int comp (const void * a, const void * b) 
{
	
	const struct tuple *elem1 = a;    
    const struct tuple *elem2 = b;

	if (elem1->timeval > elem2->timeval)
		return 1;
	else if (elem1->timeval < elem2->timeval)
		return -1;
	return 0;
}

int main(int argc, char **argv)
{

	// Process arguments, store parameters in b1, b2
	if (argc != 3) {
		printf("Pass only three arguments; b1 and b2 = parameter values\n");
		return 0;
	}
	// No error checking for bad input formats
	double b1 = strtod(argv[1], NULL);
	double b2 = strtod(argv[2], NULL);
	
	// Open input file
	FILE *fpin;
	if( (fpin = fopen("/opt/school/stat778/hw2/HW2_2018.dat", "r")) == NULL) {
		printf("File not found\n");
		exit(1);	
	}
	
	// Declare struct array to read times and censored flag 
	//  CHANGE TO MALLOC FOR DYNAMIC ARRAY SIZES
	struct tuple data[200]; // There are 200 data points
	int arrlen = 0;   // This is the next element to write	
	
	// Read input to struct array
	double time;
	int flag;
	double x1in;
	int x2in;
	while (fscanf(fpin, "%lf %i %lf %i", &time, &flag, &x1in, &x2in) == 4) {
		struct tuple t = {time, flag, x1in, x2in};
		data[arrlen] = t;
		arrlen++;
	}
	fclose(fpin);
	
	// Sort input array
	long unsigned int lflen = sizeof(struct tuple);
	qsort(data, arrlen, lflen, comp);
	
	// Declare variables for iteration
	int i = 0;
	int j = 0;
	double zb = 0;
	double cumLogLikelihood = 0;
	double riskScalarProd = 0;
	
	// Iterate through death or censor times
	for (i = 0; i < arrlen; i++) { 
		
		// Update log likelihood sum if this is a distinct death, not censored
		if (data[i].notcensored) {
			
			// scalar product of parameters and covariates for current death
			zb = b1 * data[i].x1 + b2 * data[i].x2;
			
			// Summed scalar product for risk set, all points Y >= t_j where t_j is time of 
			// current death in the loop. The current death is in the risk set.
			riskScalarProd = 0;
			for (j = i; j < arrlen; j++) {
				riskScalarProd += exp(b1 * data[j].x1 + b2 * data[j].x2);
			}
			
			cumLogLikelihood += zb - log(riskScalarProd);
		}
	}
	
	
	// print answer to stdout
	printf("Log likelihood = %lf\n", cumLogLikelihood);


	return 0;
}

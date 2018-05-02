#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

void dfb(struct tuple *data, int arrlen, double b1, double b2, double* grad1, double* grad2) {
	
	// Length of Beta vector is hardcoded as 2
	double gradientSum1 = 0;
	double gradientSum2 = 0;
	double numSum1 = 0;
	double numSum2 = 0;
	double denomSum = 0; // denominators are the same, so only one variable needed
	double zb;
	
	// Iterate through death or censor times
	for (int i = 0; i < arrlen; i++) { 
		int j = 0;
		
		// Update log likelihood sum if this is a distinct death, not censored
		if (data[i].notcensored) {
			
			// scalar product of parameters and covariates for current death
			zb = b1 * data[i].x1 + b2 * data[i].x2;
			
			// Summed scalar product for risk set, all points Y >= t_j where t_j is time of 
			// current death in the loop. The current death is in the risk set.
			numSum1 = 0;
			numSum2 = 0;
			denomSum = 0;
			for (j = i; j < arrlen; j++) {
				numSum1 += data[j].x1*exp(b1 * data[j].x1 + b2 * data[j].x2);
				numSum2 += data[j].x2*exp(b1 * data[j].x1 + b2 * data[j].x2);
				denomSum += exp(b1 * data[j].x1 + b2 * data[j].x2);
			}
			
			gradientSum1 += data[i].x1 - numSum1/denomSum;
			gradientSum2 += data[i].x2 - numSum2/denomSum;
			
			//printf("%lf %lf %lf %lf\n", data[i].x1, numSum1, denomSum, gradientSum1);
			//printf("%i %lf %lf %lf\n", data[i].x2, numSum2, denomSum, gradientSum2);
		}
	}
	
	*grad1 = gradientSum1;
	*grad2 = gradientSum2;
}



int main(int argc, char **argv)
{

	// Process arguments, store parameters in b1, b2
	if (argc != 3) {
		printf("Pass only two arguments; b1 and b2 = initial parameter value estimates\n");
		return 0;
	}
	// No error checking for bad input formats
	double b1 = strtod(argv[1], NULL);
	double b2 = strtod(argv[2], NULL);
	
	// Should be arguments, but don't have time to implement
	int maxiter = 100;
	double gamma = 0.01;
	double del = 0.000001;
	
	// Open input file
	FILE *fpin;
	if( (fpin = fopen("./HW2_2018.dat", "r")) == NULL) {
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
	
	// Step size factor is pre-set
	double grad1 = 0;
	double grad2 = 0;
	double oldb1;
	double oldb2;
	double diff;
	int i;
	
	for (i = 0; i < maxiter; i++) { // limit to 1000 iterations 
		// Find gradient at n
		dfb(data, arrlen, b1, b2, &grad1, &grad2);
		//printf("%lf %lf\n", grad1, grad2);
		
		// store old b values
		oldb1 = b1;
		oldb2 = b2;
		
		// Advance current point towards gradient
		b1 = b1 + gamma*grad1;
		b2 = b2 + gamma*grad2;
		
		diff = sqrt(pow(oldb1-b1, 2)+pow(oldb2-b2, 2));
		
		//printf("New coords: (%lf, %lf); Difference: %lf\n", b1, b2, diff);
		
		// if difference is between current and old is small enough, stop
		if (diff < del) {
			break;
		} 
		
	}
	
	if (i == maxiter) { // Loop hit its maximum iterations
		printf("Maximum iterations reached, solution did not converge\n");		
	}
	else {
		printf("Maxima: (%lf, %lf)\n", b1, b2);
	}
	
	



	return 0;
}

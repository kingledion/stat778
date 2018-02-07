#include <stdio.h>
#include <stdlib.h>

// Compare function for array sorting; lowest number in lowest index
int comp (const void * a, const void * b) 
{
    double f = *(const double*) a;
    double s = *(const double*) b;
    if (f > s) return  1;
    if (f < s) return -1;
    return 0;
}

int main(int argc, char **argv)
{
	
	// Open input file
	FILE *fp;
	if( (fp = fopen("/opt/school/stat778/hw1/HW1.dat", "r")) == NULL) {
		printf("File not found\n");
		exit(1);	
	}
	
	// Declare two arrays to read failures and censoreds 
	double failure[200]; // There are 200 data points, so I know that 
	                    // the array will be less than 200
	int lenfail = 0;   // This is the next element to write to
	
	double censored[200];
	int lencen = 0;
	
	
	// Read input to arrays
	double time;
	int flag;
	while (fscanf(fp, "%lf %i", &time, &flag) == 2) {
		if (flag == 1) {
			failure[lenfail] = time;
			lenfail++;			
		} else if (flag == 0) {
			censored[lencen] = time;
			lencen++;
		} else {
			printf("Bad line: %lf, %i\n", time, flag);
		}
	}
	
	
	// Sort arrays
	long unsigned int lflen = sizeof(double);
	qsort(failure, lenfail, lflen, comp);
	qsort(censored, lencen, lflen, comp);
	
	// Declare vars for storing survival variance function
	double survival[200];
	double survprod = 1.0;  // store the cumulative product
	double variance[200];
	double varsum = 0;      // store the cumulative summation
	
	
	
	printf("Failures\n");
	for (int i = 0; i < lenfail; i++) {
		printf("%lf\n", failure[i]);
	}
	
	

	
	return fclose(fp);
}

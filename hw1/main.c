
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Structure to store tuples of data
struct tuple {
    double timeval; 
    int censored;
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
	
	// Open input file
	FILE *fpin;
	if( (fpin = fopen("/opt/school/stat778/hw1/HW1.dat", "r")) == NULL) {
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
	while (fscanf(fpin, "%lf %i", &time, &flag) == 2) {
		struct tuple t = {time, flag};
		data[arrlen] = t;
		arrlen++;
	}
	fclose(fpin);
	
	// Sort arrays
	long unsigned int lflen = sizeof(struct tuple);
	qsort(data, arrlen, lflen, comp);
	
	
	// Declare vars for storing survival and variance functions
	double times[200];
	double survival[200];
	double survprod = 1.0;  // store the cumulative product
	double variance[200];
	double varsum = 0;      // store the cumulative summation


	
	// Declare temp variables for loop
	int dj = 0;
	int rj = 0;
	int i = 0;
	int j = 0; // i to j is a range of identical times
	
	// Write surv func = 1 at time = 0
	times[0] = 0.0;
	survival[0] = 1.0;
	variance[0] = 0.0;
	int arrwrite = 1;		// element of arrays to write to 

		
	// Loop through data performing calcs
	while (i < arrlen) { 
		
		// find range of identical time values
		j = i + 1;
		while (j < arrlen && data[j].timeval == data[i].timeval) {
			j++;
		} 
		// final value of j will be such that 200 - i is the risk set
		rj = 200-i;
		
		// Find number of deaths in range
		for (int k = i; k < j; k++) {
			dj += data[k].censored;
		}
		
		// Calculate survival dist and variance if there were deaths
		if (dj > 0) { // ignore all data is censored
			survprod = survprod * (1.0-(float)dj/rj);
			varsum += (float)dj/(rj*(rj-dj));
			times[arrwrite] = data[i].timeval; 
			survival[arrwrite] = survprod;
			variance[arrwrite] = pow(survprod, 2.0) * varsum;
			arrwrite++;
		}
		
		//reset dj and increment i to j
		i = j;
		dj = 0;
	}
	
	// Write last time, if that time censored
	
	
	if (data[arrlen-1].timeval > times[arrwrite-1]) {
		times[arrwrite] = data[arrlen-1].timeval;
		survival[arrwrite] = survprod;
		variance[arrwrite] = pow(survprod, 2.0) * varsum;
		arrwrite++;
	}
	
	// Open input file
	FILE *fpout = fopen("/opt/school/stat778/hw1/output.dat", "w");
	if(fpout == NULL) {
		printf("Output file not found\n");
	}
	
	
	// Write to stdout and file
	for (i = 0; i < arrwrite; i++) {
		printf("%lf %lf %lf\n", times[i], survival[i], variance[i]);
		if (fpout != NULL) {
			fprintf(fpout, "%lf %lf %lf\n", times[i], survival[i], variance[i]);
		}
	}
	

	
	return fclose(fpout);
}

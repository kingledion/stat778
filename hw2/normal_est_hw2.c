#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main(int argc, char **argv)
{
	// Process arguments, store number of iterations in variable 'n'
	if (argc != 2) {
		printf("Pass only one argument; n = number of iterations");
		return 0;
	}
	int n = argv[1];
	
	// Generate n random variables
	// Declare variables
	double u1; double u2;
	double z1; double z2;
	const double pi = 3.1415926535897;	
	double randnorm[n];
	int arrptr = 0;
	
	//Seed random number generator, get pi
	srand(time(NULL));


	
	while (arrptr < n) { 
		// Generate two uniform random variables
		u1 = rand()/ (double) RAND_MAX;
		u2 = rand()/ (double) RAND_MAX;
		
		// Box-Muller transform
		randnorm[arrptr] = sqrt(-2*ln(u1))*cos(2*pi*u2)
		arrptr++;
		if (arrptr < n) { // for an odd n, we cannot add off the end
			randnorm[arrptr] = sqrt(-2*ln(u2))*cos(2*pi*u1);
			arrptr++;
		}
		
	}
	
	

	
}
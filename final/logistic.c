#include <stdio.h>
#include <stdlib.h>
#include <math.h>




// return the cofactor of a matrix for the specified row and col of an (n x n) matrix A
// results is an (n-1 x n-1) matrix
void arr2d_cofac(double** A, double*** results, int n, int row, int col) {
	
	int outrow = 0;
	int outcol;
	
	for (int i = 0; i < n; i++) {
		if (i != row) {
			outcol = 0;
			for (int j = 0; j < n; j++) {
				if (j != col) {
					(*results)[outrow][outcol] = A[i][j];
					outcol++;
				}
			}
			outrow++;
		}

	}
	
}

//return the determinant of a square matrix, size (n x n)
// recursive function
double arr2d_det(double** A, int n) {
	
	//printf("In det; n = %i\n", n);
	double det = 0;
	if (n == 1) {   // determinant of a 1 x 1 matrix is the only element
		return A[0][0];
	}
	// generate empty matrix for cofactors
	double** cofac = malloc((n-1) * sizeof(double*));
	for (int i = 0; i < (n-1); i++) {
		cofac[i] = malloc((n-1) * sizeof(double));
	}
	//printf("Made cofac matrix\n");
	
	// iterate through first row and recursively call
	int sign;
	for (int i = 0; i < n; i++) {
		sign = (i%2==0)? 1: -1;
		arr2d_cofac(A, &cofac, n, 0, i);
		det += sign * A[0][i] * arr2d_det(cofac, n-1);
	}
	//printf("Finished det iteration\n");
	
	for (int i = 0; i < (n-1); i++) {
		free(cofac[i]);
	}
	free(cofac);
	//printf("Freed cofac matrix; n = %i\n", n);
	
	return det;
	
	
	
}

// adjoint of a square matrix, size (n x n)
void arr2d_adj(double** A, double*** results, int n) {
	
	if (n == 1) {   // determinant of a 1 x 1 matrix is the only element
		(*results)[0][0] = 1;
	}
	
	// generate empty matrix for cofactors
	double** cofac = malloc((n-1) * sizeof(double*));
	for (int i = 0; i < n; i++) {
		cofac[i] = malloc((n-1) * sizeof(double));
	}
	
	// iterate through first row and recursively call
	int sign;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {	
	
			sign = ((i+j)%2==0)? 1: -1;
			arr2d_cofac(A, &cofac, n, i, j);
			(*results)[j][i] += sign * arr2d_det(cofac, n-1); // built in transpose
		}
	}
	for (int i = 0; i < (n-1); i++) {
		free(cofac[i]);
	}
	free(cofac);
	
}

// inverse of a square matrix size (n x n)
void arr2d_inv(double** A, double*** results, int n) {
	//printf("In arr inv\n");
	double det = arr2d_det(A, n);
	arr2d_adj(A, results, n);
	//printf("Got adjoint\n");
	for (int i = 0; i < n; i++) {
		for (int j = 0; j< n; j++) {
			(*results)[i][j] = (*results)[i][j] / det;
		}
	}
}

// Given A is a mxn matrix and B is nxp, return A @ B; which is an m x p matrix
void arr2d_mult(double** A, double** B, double*** results, int m, int n, int p) {
	
	for (int i = 0; i < m; i++) {
		
		for (int k = 0; k < p; k++) {
			(*results)[i][k] = 0;
			for (int j = 0; j < n; j++) {
				(*results)[i][k] += A[i][j] * B[j][k];
			}
			
		}
 	}
}
 
 
void getCofactor(double** A, double** cofac, int exclRow, int exclCol, int n)
{
    int i = 0;
	int j = 0; //row index and col index to track backwards
 
    // Looping for each element of the matrix
    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < n; col++)
        {
            //  Copying into temporary matrix only those element
            //  which are not in given row and column
            if (row != exclRow && col != exclCol)
            {
                cofac[i][j++] = A[row][col];
 
                // Row is filled, so increase row index and
                // reset col index
                if (j == n - 1)
                {
                    j = 0;
                    i++;
                }
            }
        }
    }
}



/* TERMINOLOGY NOTE: FOR ALL 1D AND 2D ARRAYS
 * NUMBER OF DATA POINTS = ROWS = n
 * NUMBER OF EXPLANATORY VARIABLES = COLUMNS = k
 * i IS THE ITERATOR FOR n; j IS THE ITERATOR FOR k
 */
 

// Evaluates 1/(1-exp(-Bx))
void log_pi(double** X, double* B, double** results, int n, int k) {

	//printf("In log pi\n");
	
	for (int i = 0; i < n; i++) {
		double sum = 0;
		for (int j = 0; j < k; j++) {
			sum += B[j] * X[i][j];
		}
		(*results)[i] = 1 / (1 + exp(-1*sum));
	}
	
}

// gradient for logistic = -1 * X.T @ (y - lpi)
void log_grad(double** X, int* y, double* lpi, double** results, int n, int k) {

	//printf("In log grad\n");
	for (int j = 0; j < k; j++) {
		(*results)[j] = 0.0;
		for (int i = 0; i < n; i++) {
			(*results)[j] += -1* X[i][j] * (y[i] - lpi[i]);
		}
	}
}


void log_hess(double** X, int* y, double* lpi, double*** results, int n, int k) {
	

	// calculate  D = diagonal(pi * (1-pi))
	//printf("D\n");
	double* D = malloc(n * sizeof(double));
	for (int i = 0; i < n; i++) {
		D[i] = lpi[i] * (1 - lpi[i]);
/*		if (i < 10) {
			printf("%.3lf ", D[i]);
		}*/
	}
	//printf("\n");

	
	
	// initialize a (k x n) array as X.T @ D
	// Fill in X.T multiplied by column corresponding entry in D
	//printf("lhs\n");
	double** lhs = malloc(k * sizeof(double*));
	for (int j = 0; j < k; j++) {
		lhs[j] = malloc(n * sizeof(double));
		for (int i = 0; i < n; i++) {
			lhs[j][i] = X[i][j] * D[i];
			//if (i < 10) {
			//	printf("%.3lf ", lhs[j][i]);
			//}
		}
		//printf("\n");
	}
	//printf("Built lhs\n");
	
	// calculate X.T @ D @ X; result is a (k x k) hessian
	arr2d_mult(lhs, X, results, k, n, k);
	//printf("Array math done\n");
	
/*	printf("Hessian\n");
	for (int j = 0; j < k; j++) {
		for (int jj = 0; jj < k; jj++) {
			printf("%.3lf ", (*results)[j][jj]);
		}
		printf("\n");
	}
	printf("\n");*/
	
	
	

	free(D); 
	for (int i = 0; i < k; i++) {
		free(lhs[i]);
	}
	free(lhs);
}


// optimize beta using gradient descent
double* logisticgd(double** X, int* y, int n, int k) {
	
	//printf("Starting Gradient Descent logistic\n");
	
	double tol = 0.000001;	
	int maxiter = 1000000; // both of these should be passed as arguments
	                   // No time to figure out how function overloading works in C!

	// Using Gradient Descent
		
	// initial beta estimate to all 1
	double* B = malloc(k * sizeof(double));
	for (int j = 0; j < k; j++) {
		B[j] = 1;
	} 
	
	double delta = 1; // delta is the euclidian distance between steps
	int count = 0;
	double gamma = 0.0001;
	
	// initialize a length k array of zeros to hold results
	double* lgrad = malloc(k * sizeof(double));
	
	// length n array for probabilities
	double* lpi = malloc(n * sizeof(double));
	
	// initialize a length 
	
	while (delta > tol && count < maxiter) {
	
		log_pi(X, B, &lpi, n, k);
		log_grad(X, y, lpi, &lgrad, n, k);
		
		delta = 0;
		//printf("Next B\n");
		double* Bnext = malloc(k * sizeof(double));
		for (int j = 0; j < k; j++) {
			Bnext[j] = B[j] - gamma * lgrad[j];
			delta += pow(Bnext[j] - B[j], 2.0);
			//printf("%lf ", Bnext[j]);
		}
		//printf("\n");
		
		free(B);
		B = Bnext;
		

		delta = sqrt(delta);
/*		if (count % 100 == 0) {
			printf("B\n");
			for (int j = 0; j < k; j++) {
				printf("%.3lf ", B[j]);
			}
			
			printf("\nGradient\n");
			for (int j = 0; j < k; j++) {
				printf("%.3lf ", lgrad[j]);
			}
			
			
			
			
			printf("Step: %i complete; Last step dist: %lf\n", count, delta);
		}*/
		count += 1;
		
	}
	
	
	
	return B;
}

// optimize beta using newton-raphson
double* logisticnr(double** X, int* y, int n, int k) {
	
	//printf("Starting Newton-Raphson logistic\n");
	
	double tol = 0.000001;	
	int maxiter = 1000; // both of these should be passed as arguments
	                   // No time to figure out how function overloading works in C!

	// Using Newton-Raphson
	
	// initial beta estimate to all 0
	double* B = malloc(k * sizeof(double));
	for (int j = 0; j < k; j++) {
		B[j] = 0;
	} 
	
	
	// initialize a length k array to hold gradient
	double* lgrad = malloc(k * sizeof(double));
	
	// initialize two length (k x k) array to hold hessian and inverse
	double** lhess = malloc(k * sizeof(double*));
	double** hinv = malloc(k * sizeof(double*));
	for (int j = 0; j < k; j++) {
		lhess[j] = malloc(k * sizeof(double));
		hinv[j] = malloc(k * sizeof(double));
	}
	
	// length n array for probabilities
	double* lpi = malloc(n * sizeof(double));
	
	// initialize a (k x k) matrix tohess(X, y, lp) hold the results
	
	double* Bnext;
	
	
	double delta = 1; // delta is the euclidian distance between steps
	int count = 0;
	double h_1grad;
	
	while (delta > tol && count < maxiter) {
		
/*		printf("Current B\n");
		for (int j = 0; j < k; j++) {
			printf("%0.3lf ", B[j]);
		}
		printf("\n");*/
	
		// get value of pi for this iteration
		log_pi(X, B, &lpi, n, k); 
/*		printf("pi (first 10)\n");
		for (int j = 0; j < 10; j++) {
			printf("%0.3lf ", lpi[j]);
		}
		printf("\n"); */
		
		
		log_grad(X, y, lpi, &lgrad, n, k);
		log_hess(X, y, lpi, &lhess, n, k);
		
		//printf("Built Grad and Hess\n");
		
/*		printf("Gradient\n");
		for (int j = 0; j < k; j++) {
			printf("%.3lf ", lgrad[j]);
		}
		printf("\n");*/
		
/*		printf("Hessian\n");
		for (int j = 0; j < k; j++) {
			for (int jj = 0; jj < k; jj++) {
				printf("%.3lf ", lhess[j][jj]);
			}
			printf("\n");
		}
		printf("\n");*/
	

		arr2d_inv(lhess, &hinv, k);
		
/*		printf("\nHessian Inverse\n");
		for (int j = 0; j < k; j++) {
			for (int jj = 0; jj < k; jj++) {
				printf("%.3lf ", hinv[j][jj]);
			}
			printf("\n");
		}*/
		
		
		//printf("Inverted Hess\n");
		
		delta = 0;
		
		// vector for the next B's
		Bnext = malloc(k * sizeof(double));
		
		for (int j = 0; j < k; j++) {
			// perform hinv @ lgrad for column in question
			h_1grad = 0;
			for (int jj = 0; jj < k; jj++) {
				h_1grad += hinv[j][jj] * lgrad[jj];
			}
			Bnext[j] = B[j] - h_1grad;
			delta += pow(Bnext[j] - B[j], 2.0);
		}
		
		free(B);
		B = Bnext;
		
		count += 1;
		delta = sqrt(delta);
		//printf("Step: %i complete; Last step dist: %lf\n", count, delta);
		
	
	}
	
	return B;
	
}
	
	
	
	
	
	
	
	

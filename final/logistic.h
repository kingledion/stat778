#ifndef LOGISTIC_H_
#define LOGISTIC_H_

double* logisticgd(double** X, int* y, int n, int k);
double* logisticnr(double** X, int* y, int n, int k);
void arr2d_mult(double** A, double** B, double*** results, int m, int n, int p);
void arr2d_inv(double** A, double*** results, int n);

#endif
#ifndef DATAGEN_H_
#define DATAGEN_H_

void selectRand(double** data, int* response, double** dsub, int* rsub, int arrlen, int samplesize);

void mislabeled_1(double** data, int* response, double*** dsub, int** rsub, int arrlen, int samplesize, double mu0, double mu1);

void mislabeled_2(double** data, int* response, double*** dsub, int** rsub, int arrlen, int samplesize, double mu0, double mu1, double* B0, int c);

void mislabeled_3(double** data, int* response, double*** dsub, int** rsub, int arrlen, int samplesize, double mu0, double mu1, int c);

void mislabeled_4(double** data, int* response, double*** dsub, int** rsub, int arrlen, int samplesize, double mu0, double mu1, int c);

void standardize(double*** data, int n, int c);

int read_dataset(char* filename, double*** d, int** r, int colsize);


#endif
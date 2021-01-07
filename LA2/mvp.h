#ifndef _MVP_H_
#define _MVP_H_

//prototypes that perform the mat-vec
double* mvp1(double* mat, double* vec, int n);
double* mvp2(double** mat, double* vec, int n);

//utility functions that assign values to the matrix and vector
void assignMatrix2(double ** mat, double* vec, int n);
void assignMatrix1(double* mat, double* vec, int n);

//utility functions that print the result (only when small)
void printMatVec2(double** mat, double* vec, double* res, int n);
void printMatVec1(double* mat, double* vec, double* res, int n);

//utility function that frees 2D matrix
void freeMatrix(void** mat, int n);

#endif

#include <stdio.h>
#include <stdlib.h>
#include "mvp.h"

/*
Function prototypes for the matrix vector product
are located in the 'mvp.h' header file.
You MUST implement these functions.
*/


/*
 AssignMatrix2 takes in a double** for mat which means
 to assign the individual elements of the array all that
 needs to be done is mat[i][j] = someValue; 
 */
void assignMatrix2(double** mat, double* vec, int n){
  
  for(int i=0; i<n; i++){
    vec[i] = 1;
    for(int j=0; j<n; j++){
      if(i==j){ 
        mat[i][j] = 2;
      }
      else if(i==j+1||j==i+1){
        mat[i][j] = 1;
      }
      else{
        mat[i][j] = 0;
      }
    }
  }
}


/*
 AssignMatrix1 takes a double* for mat which makes it a
 little more cumbersome to assign each individual element.
 Since it is a single dimensional array of pointers to assign
 the value I used *(mat+i*n+j) which dereferences the value in
 the array, then using i*n moves to the right row and +J moves
 to the right column.
 */
void assignMatrix1(double* mat, double* vec, int n){
  for(int i=0; i<n; i++){
    vec[i]=1;
    for(int j=0; j<n; j++){
      if(i==j){ 
        *(mat + i*n + j) = 2;
      }
      else if(i==j+1||j==i+1){
        *(mat + i*n + j) = 1;
      }
      else{
        *(mat + i*n + j) = 0;       
      }
    }
  }
}

/*
 Both mvp1 and mvp2 calculate the product of each row of the matrix and assigns
 them to the corresponding column of the solution vector. The only difference
 being the way that the elements of mat are referenced.
 */
double* mvp1(double* mat, double* vec, int n){
  double prod;
  double* res = NULL;
  res = (double*)malloc(n*sizeof(double));
 
  for(int i=0; i<n; i++){
    prod = 0;
    for(int j=0; j<n; j++){
      prod+=vec[i]*mat[i*n + j];
    }
    res[i] = prod;
  }
  return res; 
}

double* mvp2(double** mat, double* vec, int n){
  double prod;
  double* res = NULL;
  res = (double*)malloc(n*sizeof(double));
 
  for(int i=0; i<n; i++){
    prod = 0;
    for(int j=0; j<n; j++){
      prod+=vec[j]*mat[i][j];
    }
    res[i] = prod;
  }
  return res;
}

/*
 Using nested for loops to print the array allows to print the entire row of
 the original matrix, then the vector, and then the resulting vector using an if
 statement to place an = at roughly the midpoint of the outputs.
 */
void printMatVec2(double** mat, double* vec, double* res, int n){
   for(int i=0; i<n; i++){
     for(int j=0; j<n; j++){
       printf("%.2f ", mat[i][j]);
     }
     printf("  %.2f", vec[i]);
     if(i==(n/2)){void assignMatrix1(double* mat, double* vec, int n){
  for(int i=0; i<n; i++){
    vec[i]=1;
    for(int j=0; j<n; j++){
      if(i==j){ 
        *(mat + i*n + j) = 2;
      }
      else if(i==j+1||j==i+1){
        *(mat + i*n + j) = 1;
      }
      else{
        *(mat + i*n + j) = 0;       
      }
    }
  }
}void assignMatrix1(double* mat, double* vec, int n){
  for(int i=0; i<n; i++){
    vec[i]=1;
    for(int j=0; j<n; j++){void assignMatrix1(double* mat, double* vec, int n){
  for(int i=0; i<n; i++){
    vec[i]=1;
    for(int j=0; j<n; j++){
      if(i==j){ 
        *(mat + i*n + j) = 2;
      }
      else if(i==j+1||j==i+1){
        *(mat + i*n + j) = 1;
      }void assignMatrix1(double* mat, double* vec, int n){
  for(int i=0; i<n; i++){
    vec[i]=1;
    for(int j=0; j<n; j++){
      if(i==j){ 
        *(mat + i*n + j) = 2;
      }
      else if(i==j+1||j==i+1){
        *(mat + i*n + j) = 1;
      }
      else{
        *(mat + i*n + j) = 0;       
      }
    }
  }
}
      else{
        *(mat + i*n + j) = 0;       
      }
    }
  }
}
      if(i==j){ 
        *(mat + i*n + j) = 2;
      }
      else if(i==j+1||j==i+1){
        *(mat + i*n + j) = 1;
      }
      else{
        *(mat + i*n + j) = 0;       
      }
    }
  }
}
       printf(" = %.2f\n", res[i]);
     }
     else{
      printf("   %.2f\n", res[i]);
     }
   }
}

void printMatVec1(double* mat, double* vec, double* res, int n){
  for(int i=0; i<n; i++){
    for(int j=0; j<n; j++){
       printf("%.2f ", *(mat + i*n + j));
     }
     printf("  %.2f", vec[i]);
     if(i==(n/2)){
       printf(" = %.2f\n", res[i]);
     }
     else{
      printf("   %.2f\n", res[i]);
     }
   }
}

/*
 This frees each individual row of mat before freeing the array that held all
 of the row references.
 */
void freeMatrix(void** mat, int n){
  for(int i=0; i<n; i++){
    free(mat[i]);
   }
   free(mat);
}




//Main function
int main(int argc, char **argv){
  printf("=============MVP2=============\n");
  
  int n = 12;

  double** mat2 = NULL;

  mat2 = (double**)malloc(n*sizeof(double*));
  for(int i=0; i<n; i++){
    mat2[i] = (double*)malloc(n*sizeof(double));
  } 

  double* vec = NULL;
  vec = (double*)malloc(n*sizeof(double));
  
  assignMatrix2(mat2, vec, n);
  double* res = NULL;  
  res = mvp2(mat2, vec, n);
  printMatVec2(mat2, vec, res, n);
  freeMatrix((void**)mat2, n);
  free(res);
  
  printf("=============MVP1=============\n");
  double* mat1 = NULL;
  mat1 = (double*)malloc(n*n*sizeof(double));
  assignMatrix1(mat1, vec, n);
  res = NULL;
  res = mvp1(mat1, vec, n);
  printMatVec1(mat1, vec, res, n);
  free(mat1);
  free(vec);
  free(res);
  return 0;
}

 

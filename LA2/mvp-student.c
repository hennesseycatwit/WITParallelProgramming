#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

/*
Function prototypes for the matrix vector product
are located in the 'mvp.h' header file.
You MUST implement these functions.
*/





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
        mat[i*n + j] = 2;

      }
      else if(i==j+1||j==i+1){
         mat[i*n + j] = 1;
      }
      else{
        mat[i*n + j] = 0;       
      }
    }
  }
}

/*
 Both mvp1 and mvp2 calculate the product of each row of the matrix and assigns
 them to the corresponding column of the solution vector. The only difference
 being the way that the elements of mat are referenced.
 */
double* mvp1(double* mat, double* vec, int numrows, int n){
  double prod;
  double* res = NULL;
  res = (double*)malloc(numrows*sizeof(double));
 
  for(int i=0; i<numrows; i++){
    prod = 0;
    for(int j=0; j<n; j++){
      prod+=vec[i]*mat[i*n + j];
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


//Main function
int main(int argc, char **argv){
  int numranks, rank, n;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numranks);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double t1 = MPI_Wtime();
  

  double* myMat = NULL;
  double* mat1 = NULL;
  double* vec = NULL;  
  if(rank==0){
    n=40000;
    mat1 = (double*)malloc(n*n*sizeof(double));
    vec=(double*)malloc(n*sizeof(double));
    assignMatrix1(mat1, vec, n); 
  }
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
  int numRows = n/numranks;
  int sendcount = numRows*n;

  if(n%numranks!=0){
    printf("This is not going to work");
    MPI_Finalize();
    return 0;
  }


  if(rank!=0){
    vec=(double*)malloc(n*sizeof(double));
  }

  myMat = (double*)malloc(sendcount*sizeof(double));
  
  MPI_Bcast(vec, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Scatter(mat1, sendcount, MPI_DOUBLE, myMat, sendcount, MPI_DOUBLE, 0, MPI_COMM_WORLD);

   

  double* myRes = mvp1(myMat, vec, numRows, n);  
   
  double* finalRes = (double*)malloc(n*sizeof(double));
  MPI_Gather(myRes, numRows, MPI_DOUBLE, finalRes, numRows, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  
  double t2 = MPI_Wtime();
  if(rank==0){ 
    printf("Total Time: %f, N: %d\n", t2-t1, n);
    if(n<50){
      printMatVec1(mat1, vec, finalRes, n);
    }
    free(mat1);
    free(finalRes);
  }
  

  MPI_Finalize();
  free(myRes);
  free(vec);
  free(myMat);
}

 

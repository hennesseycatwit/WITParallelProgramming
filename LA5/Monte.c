#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "mpi.h"

int main(int argc, char **argv) {
  int rank, numranks;
  double x, y, val, pi;
  long n, h;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numranks);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double start=MPI_Wtime();
  n=20000000000;
  
  long chunk = n/numranks;
  long myStart = chunk*rank;
  long myEnd = myStart+chunk-1;
  if(rank==numranks-1){
    myEnd = n-1;
  }
  h=0;
  for(long i=myStart; i<=myEnd; i++){
    x=(double)rand()/(double)RAND_MAX;
    y=(double)rand()/(double)RAND_MAX;
    val=(x*x)+(y*y);
    if(val<=1){
      h++;
    }
  }
  MPI_Allreduce(MPI_IN_PLACE, &h, 1, MPI_LONG, MPI_SUM, MPI_COMM_WORLD);
  pi=0;
  pi=(4.0*((double)h/(double)n));
  double end=MPI_Wtime();
  printf("pi=%.12f    Actual=%.12f    Time=%f\n", pi, acos(-1), end-start);

  MPI_Finalize();
  return 0;
}
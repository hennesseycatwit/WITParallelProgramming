#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"




int main (int argc, char **argv){

  int numranks, rank, N, myRes, res;
  
  double t1 = MPI_Wtime();
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numranks);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int* a = NULL;
  int* b = NULL; 
  int* myA = NULL;
  int* myB = NULL; 

  if(rank==0){
    N=1600000000;
    a = (int*)malloc(N*sizeof(int));
    b = (int*)malloc(N*sizeof(int));

    for(int i=0; i<N; i++){
      a[i] = b[i] = 2;
    }
  }
  MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
  int numElements = N/numranks;
  int rem = N%numranks;
  int scount = 0;
  int* sendcount = (int*)malloc(numranks*sizeof(int));
  int* disp = (int*)malloc(numranks*sizeof(int));
  
  for(int i=0; i<numranks; i++){  //sets the sendcount at the current 
    sendcount[i] = numElements;   //index to N/numranks

    if(rem>0){             //if N%numranks is greater than 0 
      sendcount[i]++;      //add one to sendcount at i and 
      rem--;               //subtract 1 from the remainder
    }

    disp[i] = scount;    //displacement[i] to sum of sendcounts
    scount+=sendcount[i];//add current sendcount to sum of sendcounts
  }
    

  myA = (int*)malloc(sendcount[rank]*sizeof(int));
  myB = (int*)malloc(sendcount[rank]*sizeof(int));
  MPI_Scatterv(a, sendcount, disp, MPI_INT, myA, sendcount[rank], MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Scatterv(b, sendcount, disp, MPI_INT, myB, sendcount[rank], MPI_INT, 0, MPI_COMM_WORLD);
  res=0;
  
  for(int i=0; i<sendcount[rank]; i++){
    res+=myA[i]*myB[i];
  }
  MPI_Allreduce(MPI_IN_PLACE, &res, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  double t2=MPI_Wtime();
  
  printf("Rank: %d, result: %d, time: %f\n", rank, res, t2-t1);
  
  if(rank == 0){
    free(a);
    free(b);
  }

  MPI_Finalize();
  free(myA); 
  free(myB);
}

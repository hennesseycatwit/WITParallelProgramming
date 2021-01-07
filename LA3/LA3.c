#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int is_prime(int n);

int main(int argc, char** argv){
    int n, numranks, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   
    n=10000000;
    int chunk = n/numranks;
    int myStart = chunk*rank;
    int myEnd = myStart+chunk-1;
    if(rank==numranks-1){
        myEnd = n-1;
    }
    int numprimes = 0;
    int i;
    for (i = myStart; i <= myEnd; i++){
        if (is_prime(i) == 1)
            numprimes ++;
    }
    MPI_Allreduce(MPI_IN_PLACE, &numprimes, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    //this should be 664579
    printf("Number of Primes: %d\n",numprimes);
    MPI_Finalize();

}

int is_prime(int n)
{
    /* handle special cases */
    if      (n == 0) return 0;
    else if (n == 1) return 0;
    else if (n == 2) return 1;

    int i;
    for(i=2;i<=(int)(sqrt((double) n));i++)
        if (n%i==0) return 0;

    return 1;
}

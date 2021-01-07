#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char **argv)
{

    //Usual MPI init
    double t1 = MPI_Wtime();
    MPI_Init(&argc, &argv);
    int rank, numranks;
    MPI_Comm_size(MPI_COMM_WORLD, &numranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status stat;

    int N;
    int result = 0;

    //simulate I/O or user input
    if (rank == 0)
    {
        N=1600000000;
    }

    //give everyone N
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (N % numranks != 0)
    {
        if (rank == 0)
        {
            printf("number of ranks should divide number of elements evenly\n");
        }
        MPI_Finalize();
        return 0;
    }

    //the pointers to the full array must exist for everyone
    int *a;
    int *b;

    //simulate I/O or user input
    if (rank == 0)
    {
        //only rank 0 allocates the full array
        a = (int *)malloc(N * sizeof(int));
        b = (int *)malloc(N * sizeof(int));

        //assign values to arrays
        for (int i = 0; i < N; i++)
        {
            a[i] = b[i] = 2;
        }
    }

    //must be evenly divisable
    int myN = N / numranks;

    int sendcounts = myN;

    //each rank needs a place to put the small part of the full array
    //includes rank 0
    int *mya = (int *)malloc(myN * sizeof(int));
    int *myb = (int *)malloc(myN * sizeof(int));

    //scatter small chunks of the full arrays to everyone
    MPI_Scatter(a, sendcounts, MPI_INT, mya, sendcounts, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(b, sendcounts, MPI_INT, myb, sendcounts, MPI_INT, 0, MPI_COMM_WORLD);

    // do the do product
    for (int i = 0; i < myN; i++)
    {
        result += mya[i] * myb[i];
    }

    double t2 = MPI_Wtime();


    printf("Rank: %d, result: %d\n", rank, result);

    //result is split between ranks
    //MPI_IN_PLACE uses result as the send and recv for the reduction
    MPI_Allreduce(MPI_IN_PLACE, &result, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    printf("Rank: %d Result: %d, time: %f\n", rank, result, t2-t1);

    MPI_Finalize();
}
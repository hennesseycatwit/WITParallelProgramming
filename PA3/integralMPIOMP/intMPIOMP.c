#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

double func(double x);

int main(int argc, char **argv)
{
    int N;
    double a, b;
    int rank, numranks;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Request request;
    MPI_Status stat;

    //assume only rank 0 has these values to start with
    if (rank == 0)
    {
        N = 100000;
        a = 1;
        b = 2;
    }

    //send the three initial values to all threads
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&b, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double width = (b - a) / N;
    double result = 0;
    double myresult = 0;

    //compute mystart and myend
    int numeles = N / numranks;
    int mystart = rank * numeles + 1;
    int myend = mystart + numeles - 1;
    if (rank == numranks - 1)
        myend = N;
    printf("rank: %d, mystart: %d, myend %d\n", rank, mystart, myend);

    //sum of height

#pragma omp parallel for default(shared) reduction(+ \
                                                   : myresult)
    for (int i = mystart; i <= myend; i++)
    {
        myresult += func(a + (i - 1) * width);
    }
    //multiply by widths
    myresult = myresult * width;

    //reduce
    MPI_Reduce(&myresult, &result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("\nIntegral of x^2 from 1 to 2: %f\n\n", result);
    }

    MPI_Finalize();
}

double func(double x)
{

    return x * x;
}

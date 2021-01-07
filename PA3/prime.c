#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int is_prime(int n);

int main(int argc, char **argv)
{

    //initialize MPI
    MPI_Init(&argc, &argv);
    int rank, numranks;
    MPI_Comm_size(MPI_COMM_WORLD, &numranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status stat;

    int n = 25000000;
    int range[2];
    int numprimes = 0;
    int work = 50000; //amount of work to send each time, work<<n
    int current = 0;  //bookkeeping to send the correct work when a new rank needs it

    if (rank == 0) //master
    {
        double starttime = MPI_Wtime();
        //initial sends, give everyone a small bit of work
        for (int i = 1; i < numranks; i++)
        {
            range[0] = current;            //i=1, 0
            range[1] = current + work - 1; //i=1  0+100-1 -> 99
            current += work;
            MPI_Send(range, 2, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        //wait for workers to send results and then send them more work to do
        int done = 0;
        while (1)
        {
            //all the book keeping takes place here
            int val = 0;
            MPI_Recv(&val, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &stat);
            numprimes += val;

            if (current == -1) //no work left...
            {
                range[0] = -1; //invalid mystart value, acts as the done signal to the worker
                done++;
            }
            else //there is more work to do (more work to hand out to workers)
            {
                range[0] = current;
                range[1] = current + work - 1;
                current += work;
            }
            if (range[1] >= n) //less than 'work' left
            {
                range[1] = n;
                current = -1; //invalid current value
            }

            MPI_Send(range, 2, MPI_INT, stat.MPI_SOURCE, 0, MPI_COMM_WORLD);
            if (done == numranks - 1)
                break;
        }
        double endtime = MPI_Wtime();
        printf("Number of Primes: %d\n", numprimes);
        printf("Total Time: %.5f\n", endtime - starttime);
    }

    if (rank != 0) //worker
    {
        double starttime = MPI_Wtime();
        while (1) //infinite loop because we may not know how many pieces of work we will get
        {
            MPI_Recv(range, 2, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);

            if (range[0] == -1) //got the terminate signal from master
                break;

            //compute number of primes
            numprimes = 0;
            for (int i = range[0]; i <= range[1]; i++)
            {
                if (is_prime(i) == 1)
                    numprimes++;
            }
            MPI_Send(&numprimes, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
        double endtime = MPI_Wtime();
        printf("Rank: %d Time: %.5f\n", rank, endtime - starttime);
    }

    MPI_Finalize();
}

int is_prime(int n)
{
    /* handle special cases */
    if (n == 0)
        return 0;
    else if (n == 1)
        return 0;
    else if (n == 2)
        return 1;

    int i;
    for (i = 2; i <= (int)(sqrt((double)n)); i++)
        if (n % i == 0)
            return 0;

    return 1;
}

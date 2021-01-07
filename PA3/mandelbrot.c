#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>

extern void matToImage(char *name, int *mat, int *dims);


int main(int argc, char **argv)
{

    int rank, numranks;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status stat;


    int *mat;
    //3:2, x:y
    int height = 12000; //y
    int width = 18000;  //x

    mat = (int *)malloc(height * width * sizeof(int));
   
    //variable setup
    int maxiter = 255;
    double r_start = -2;
    double r_end = 1;
    double i_start = -1;
    double i_end = 1;
    int iter;
    double x, y;   //real and img parts of Z
    double x0, y0; //real and img parts of C
    int numrows = height / numranks;
    if (height % numranks != 0){
        if (rank == 0){
            printf("Number of ranks must evenly divide rows\n");
        }
        MPI_Finalize();
        return 1;
    }

    int range[2];
    int work = 100; //amount of work to send each time, work<<n
    int current = 0;  //bookkeeping to send the correct work when a new rank needs it
    int* matchunk = NULL;
    int disp=0;


    if (rank == 0) //master
    {
        double starttime = MPI_Wtime();
        //initial sends, give everyone a small bit of work
        for (int i = 1; i < numranks; i++)
        {
            range[0] = current;            //i=1, 0
            range[1] = current + work - 1; //i=1  0+work0-1 -> 99
            current += work;
            MPI_Send(range, 2, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        //wait for workers to send results and then send them more work to do
        int done = 0;
        while (1)
        {

            //all the book keeping takes place here
            matchunk=NULL;
            matchunk=(int*)malloc((work)*width*sizeof(int));
            MPI_Recv(matchunk, (work)*width, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &stat);
            for(int i=disp; i<=disp+work-1&&i<height; i++){
              for(int j=0; j<width; j++){
                mat[i*width+j]=matchunk[(i-disp)*width+j];
              }
            }
            
            //printf("this loop ok Start %d end %d\n", range[0], range[1]);
            if (current == -1) //no work left...
            {
                range[0] = -1; //invalid mystart value, acts as the done signal to the worker
                done++;
            }
            else //there is more work to do (more work to hand out to workers)
            {
                range[0] = current;
                range[1] = current + work - 1;
                disp=current;
                current += work;
            }
            if (range[1] > height) //less than 'work' left
            {
                range[1] = height-1;
                current = -1; //invalid current value
            }
            
            MPI_Send(range, 2, MPI_INT, stat.MPI_SOURCE, 0, MPI_COMM_WORLD);
            
            if (done == numranks - 1)
                break;
        }
        double endtime = MPI_Wtime();
        int dims[2] = {height, width};
        matToImage("Mandelbrot.jpg", mat, dims);
        printf("Total Time: %.5f\n", endtime - starttime);
    }
    else{ //worker
      double starttime = MPI_Wtime();
      while(1){
        MPI_Recv(range, 2, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);
        if (range[0] == -1) //got the terminate signal from master
                break;
        matchunk=(int*)malloc((work)*width*sizeof(int));
#pragma omp parallel private(iter, x, y, x0, y0)
  {
    int tid=omp_get_thread_num();
    double threadstart=omp_get_wtime();

#pragma omp for nowait schedule(dynamic, 1)
          for(int i=range[0];i<=range[1];i++){
            for (int j=0;j<width;j++){
                //I have a pixel (i,j)
                //convert to x0,y0 on real-img plane
                x0 = r_start + (1.0 * j / width) * (r_end - r_start);
                y0 = i_start + (1.0 * i / height) * (i_end - i_start);
                x = 0;
                y = 0;
                iter = 0;
                while (iter < maxiter)
                {
                    double xt = x * x - y * y + x0; //compute real part of Z
                    double yt = 2.0 * x * y + y0;   //computer img part of Z
                    x = xt;
                    y = yt;
                    iter++;
                    if (x * x + y * y > 4)
                    {
                        break;
                    }
                }
                matchunk[(i-range[0]) * width + j] = iter;
                
            }
          }
          
        }
        MPI_Send(matchunk, (work)*width, MPI_INT, 0, 0, MPI_COMM_WORLD);
        double endtime = MPI_Wtime();
        printf("Rank: %d Time: %.5f\n", rank, endtime - starttime);
      }
    }


    free(mat);

    MPI_Finalize();
    return 0;
}


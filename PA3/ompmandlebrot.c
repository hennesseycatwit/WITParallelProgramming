#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

extern void matToImage(char *name, int *mat, int *dims);

int main(int argc, char** argv){

  int nx = 6000;
  int ny = 4000;
  int maxiter = 255;
  int *mat;
  double area, x0, y0, x, y;
  double r_start, r_end, i_start, i_end;

  r_start=-2;
  r_end=1;
  i_start=-2;
  i_end=1;

  mat=(int*)malloc(nx*ny*sizeof(int));

  int iter;
#pragma omp parallel private(iter, x, y, x0, y0)
  {

    int tid=omp_get_thread_num();
    double starttime=omp_get_wtime();

#pragma omp for nowait schedule(dynamic, 1)
      for(int i=0; i<ny; i++){
        for(int j=0; j<nx; j++){
          //convert
          x0 = r_start + (j/(1.0*nx)) * (r_end - r_start);
          y0 = i_start + (i /(1.0*ny)) * (i_end - i_start);
          
          x=0;
          y=0;

          iter=0;

          while(iter<maxiter){
            double xt=x*x-y*y+x0; //compute real part of Z
            double yt=2.0*x*y+y0;   //computer img part of Z
            x = xt;
            y = yt;
            iter++;
            if (x*x+y*y>4)
            {
              break;
            }
          }
          mat[i*nx+j]=iter;
        }
      }
      double endtime = omp_get_wtime();
      printf("%d: time: %.5f\n", tid, endtime-starttime);
  }
  int dims[2] = {ny, nx};
  matToImage("image.jpg", mat, dims);
}
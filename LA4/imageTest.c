#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

extern int *imageToMat(char *name, int *dims);
extern void matToImage(char *name, int *mat, int *dims);

int meanFilter(int *mat, int i, int j, int k, int kDim, int width);

int main(int argc, char **argv) {
  int rank, numranks;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numranks);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int *mat = NULL;
  int *newMat = NULL;
  int *dims = NULL;

  dims = (int *)malloc(sizeof(*dims) * 2);

  if (rank == 0) {
    char *name = "DSC03309.jpg";
    mat = imageToMat(name, dims);
  }

  double start = MPI_Wtime();
  MPI_Bcast(dims, 2, MPI_INT, 0, MPI_COMM_WORLD);
  int height = dims[0];
  int width = dims[1];
  printf("Dims: %d %d Rank: %d\n", height, width, rank);
  int matSize = height * width;

  /*
  Splitting the image matrix up by number of rows made much more sense than
  splitting by number of pixels by doing this it limits the amount of possible
  edge cases to check for.

  The sendcount is the number of pixels being recieved by the MPI_Gatherv, the
  disp is the number of pixels preceding the current rank, and the rowDisp is
  the number of rows before preceding the current rank. This makes looping
  through the values much easier.
  */

  int numrows = height / numranks;
  int rem = height % numranks;
  int *sendcount = (int *)malloc(numranks * sizeof(int));
  int *disp = (int *)malloc(numranks * sizeof(int));

  for (int i = 0; i < numranks; i++) {
    if (rem != 0 && i == numranks - 1) {

      sendcount[i] = (numrows + rem) * width;
      disp[i] = disp[i - 1] + numrows * width;

    } else {
      sendcount[i] = numrows * width;
      if (i == 0)
        disp[i] = 0;
      else
        disp[i] = disp[i - 1] + sendcount[i];
    }
  }
  int rowDisp = disp[rank] / width;

  if (rank != 0) {
    mat = (int *)malloc(matSize * sizeof(*mat));
  }

  MPI_Bcast(mat, matSize, MPI_INT, 0, MPI_COMM_WORLD);
  printf("disp: %d Rank %d\n", rowDisp, rank);
  newMat = (int *)malloc(sendcount[rank] * sizeof(int));

  int k = 20;
  int kDim = (2 * k + 1) * (2 * k + 1);
  // Below are four statements to check all of the edge cases that need to be
  // handled so the kernel does not check values that are out of bounds.
  if (numranks == 1) {
    for (int i = k; i < height - k; i++) {
      for (int j = k; j < width - k; j++) {
        newMat[i * width + j] = meanFilter(mat, i, j, k, kDim, width);
      }
    }
  }

  else if (rank == 0) {
    for (int i = k + 1; i < numrows; i++) {
      for (int j = k; j < width - k; j++) {
        newMat[i * width + j] = meanFilter(mat, i, j, k, kDim, width);
      }
    }
  } else if (rank == numranks - 1) {
    for (int i = rowDisp; i < height - k; i++) {

      for (int j = k; j < width - k; j++) {
        newMat[(i - rowDisp) * width + j] =
            meanFilter(mat, i, j, k, kDim, width);
      }
    }
  } else {
    for (int i = rowDisp; i < rowDisp + numrows; i++) {
      for (int j = k; j < width - k; j++) {
        newMat[(i - rowDisp) * width + j] =
            meanFilter(mat, i, j, k, kDim, width);
      }
    }
  }

  printf("Rank: %d made it through doing math\n", rank);

  MPI_Gatherv(newMat, sendcount[rank], MPI_INT, mat, sendcount, disp, MPI_INT, 0, MPI_COMM_WORLD);
  double end = MPI_Wtime();
  if (rank == 0) {
    printf("Total time: %f", end - start);
    matToImage("processedImage6.jpg", mat, dims);
  }

  MPI_Finalize();
  free(mat);
  free(newMat);
  return 0;
}

meanFilter(int *mat, int i, int j, int k, int kDim, int width) {
  int index = i * width + j;
  int sum = 0;

  for (int u = i - k; u <= i + k; u++) {
    for (int v = j - k; v <= j + k; v++) {

      int pixel = mat[u * width + v];
      sum += pixel;
    }
  }

  mat[index] = sum / kDim;
  if (mat[index] > 255)
    mat[index] = 255;
  return mat[index];
}
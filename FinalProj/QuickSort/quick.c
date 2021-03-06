#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


void swap(int *a, int *b);
int partition(int *arr, int low, int high);
void quickSort(int *arr, int low, int high);
int* merge(int* arr1, int n1, int* arr2, int n2);

int main(int argc, char **argv) {
  int n, myArrSize, newSize, mergeSize, numranks, rank;
  int* arr = NULL;
  int* myArr = NULL;
  int* merger = NULL;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numranks);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  n=72000000;
  myArrSize = n/numranks;
  if(n%numranks){
    printf("please use a number of ranks which n is divisible by");
    MPI_Finalize();
    return 0;
  }

  arr = (int*)malloc(n*sizeof(int));
  myArr = (int*)malloc(myArrSize*sizeof(int));

  if(rank==0){
    for (int i = 0; i < n; i++) {
      arr[i] = (rand() % (n - 0 + 1)) + 0;
    }
  }
  double start = MPI_Wtime();

  MPI_Scatter(arr, myArrSize , MPI_INT, myArr, myArrSize, MPI_INT, 0 , MPI_COMM_WORLD);
  newSize = myArrSize;

  quickSort(myArr, 0, myArrSize-1);

  //This loop merges all of the sorted ranks into the ranks that are multiples of the values of log2(x) until all ranks are merged into rank 0
  //with the way that the pattern works after each iteration the remaining ranks will be seperated by log2(n-1) which is why the "send ranks"
  //are always double i.

  for (int i = 1; i < numranks; i*=2) {
    if (rank % (2*i) != 0) {
      MPI_Send(myArr, newSize, MPI_INT, rank-i, 0, MPI_COMM_WORLD);
      break;
    }
    // id is multiple of 2*step: merge in myArr from id+step (if it exists)
    if (rank+i < numranks) {
      // compute size of myArr to be received
      if(n >= myArrSize * (rank+2*i)){
        mergeSize=myArrSize*i;
      }
      else{
        mergeSize=n-myArrSize*(rank+i);
      }
      // receive myArr
      merger = (int*)malloc(mergeSize*sizeof(int));
      MPI_Recv(merger, mergeSize, MPI_INT, rank+i, 0, MPI_COMM_WORLD, NULL);
      // merge and free memory
      arr = merge(myArr, newSize, merger, mergeSize);
      free(myArr);
      free(merger);
      myArr = arr;
      newSize += mergeSize;
    }
  }
  double end = MPI_Wtime();
  if(rank==0){
    printf("time: %f", end-start);  
    free(arr);
  }

  
  MPI_Finalize();

  return 0;
}

void swap(int* a, int* b) {
  int t = *a;
  *a = *b;
  *b = t;
}

int partition(int* arr, int low, int high) {
  int pivot = arr[high]; 
  int i = (low - 1);  
  for (int j = low; j <= high - 1; j++) {
    if (arr[j] < pivot) {
      i++; 
      swap(&arr[i], &arr[j]);
    }
  }
  swap(&arr[i + 1], &arr[high]);
  return (i + 1);
}


void quickSort(int* arr, int low, int high) {
  if (low < high) {
    int par = partition(arr, low, high);
    quickSort(arr, low, par - 1);
    quickSort(arr, par + 1, high); 
  }
}

//This method is borrowed from merge.c
int* merge(int* arr1, int n1, int* arr2, int n2){
  int* result = (int*)malloc((n1+n2)*sizeof(int));
  int i = 0;
  int j = 0;
  int k = 0;
  while(i<n1&&j<n2){
    if(arr1[i]<=arr2[j]){
      result[k] = arr1[i];
      i++;
    }
    else{
      result[k] = arr2[j];
      j++;
    }
    k++;
  }
  while (i < n1) { 
    result[k] = arr1[i]; 
    i++; 
    k++; 
  } 
  while (j < n2) { 
    result[k] = arr2[j]; 
    j++; 
    k++; 
  } 
  return result;
}

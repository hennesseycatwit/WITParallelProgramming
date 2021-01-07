#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


void merge(int* arr, int l, int m, int r);
void mergeSort(int* arr, int l, int r);
int* arrMerge(int* arr1, int n1, int* arr2, int n2);

int main(int argc, char** argv){
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

  arr = (int*)malloc(n * sizeof(int));
  myArr = (int*)malloc(myArrSize*sizeof(int));

  if(rank==0){
    for (int i = 0; i < n; i++) {
      arr[i] = (rand() % (n - 0 + 1)) + 0;
    }
  }
  double start = MPI_Wtime();

  MPI_Scatter(arr, myArrSize , MPI_INT, myArr, myArrSize, MPI_INT, 0 , MPI_COMM_WORLD);
  newSize = myArrSize;

  mergeSort(myArr, 0, myArrSize-1);
  //This loop merges all of the sorted ranks into the ranks that are multiples of the values of log2(x) until all ranks are merged into rank 0
  //with the way that the pattern works after each iteration the remaining ranks will be seperated by log2(n-1) which is why the "send ranks"
  //are always double i.
  for (int i = 1; i < numranks; i*=2) {
    if (rank % (2*i) != 0) {
      MPI_Send(myArr, newSize, MPI_INT, rank-i, 0, MPI_COMM_WORLD);
      break;
    }
    if (rank+i < numranks) {
      // compute size of myArr to be received
      if(n >= myArrSize * (rank+2*i)){
        mergeSize=myArrSize*i;
      }
      else{
        mergeSize=n-myArrSize*(rank+i);
      }
      //receive myArr
      merger = (int*)malloc(mergeSize*sizeof(int));
      MPI_Recv(merger, mergeSize, MPI_INT, rank+i, 0, MPI_COMM_WORLD, NULL);
      // merge and free memory
      arr = arrMerge(myArr, newSize, merger, mergeSize);
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
}

void mergeSort(int* arr, int l, int r){
  if(l<r){
    int m = l+(r-l)/2;
    mergeSort(arr, m+1, r);
    mergeSort(arr, l, m);
    merge(arr, l, m, r);
  }
}

void merge(int* arr, int l, int m, int r){
  int lSize, rSize;
  int* left = NULL;
  int* right = NULL;
  int* temp = NULL;
  lSize = m-l+1;
  rSize = r-m;
  left = (int*)malloc(lSize*sizeof(int));
  right = (int*)malloc(rSize*sizeof(int));
  temp = (int*)malloc((rSize+lSize)*sizeof(int));

  for(int i=0; i<lSize; i++) left[i] = arr[l+i];

  for(int i=0; i<rSize; i++) right[i] = arr[m+1+i];

  temp = arrMerge(left, lSize, right, rSize);

  for(int i=l; i<=r; i++) arr[i] = temp[i-l];
  free(temp);
}

//This function is to merge two arrays which is needed in both the merge sort, as well as merging the sorted lists from two ranks.
int* arrMerge(int* arr1, int n1, int* arr2, int n2){
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

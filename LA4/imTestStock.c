#include<stdio.h>
#include<stdlib.h>


extern int* imageToMat(char* name,int* dims);
extern void matToImage(char* name, int* mat, int* dims);

int main(int argc, char** argv){

  int* mat=NULL;
  int* dims=NULL;
  dims=(int*) malloc(sizeof(*dims)*2);
  char *name="Circuit.jpg";
  mat=imageToMat(name,dims);

  int k=2;
  int kDim=(2*k)*(2*k)+1;
  for(int i=k; i<dims[0]-k; i++){
    for(int j=k; j<dims[1]-k; j++){
      
      int index=i*dims[1]+j;
      int sum=0;

      for(int u=i-k; u<=i+k; u++){
        for(int v=j-k; v<=j+k; v++){

          int pixel = mat[u * dims[1] + v];
          sum+=pixel;

        }
      }

      mat[index] = sum/kDim;
      if(mat[index]>255) mat[index]=255;
    } 
    
  }
  matToImage("processedImage.jpg",mat,dims);

  

  return 0;
}

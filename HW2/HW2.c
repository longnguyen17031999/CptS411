//This is prefix sum algorithm - Up-sweep + Down-sweep

#include<stdio.h>
#include<omp.h>
#include<math.h>
#include<stdlib.h>


//NOTE: please use this command to compile: gcc HW2.c -fopenmp -o run -lm

int main(int argc, char* argv[]){

    // Array A - as described in lecture - used as test case
    int A[8] = {3,1,1,7,2,5,9,2};
    //n = length of A
    int n = sizeof(A)/sizeof(A[0]);

    //Print out the result for testing purpose
    printf("Original Array A = [");
    for (int i = 0; i < n; i++){
        if (i == n-1)
            printf("%d", A[i]);
        else
            printf("%d, ", A[i]);
    }
    printf("]\n");

    //Up-sweep
    int *B[(int)(log2(n)+1)]; //array B to hold up-sweep part
    //Initialize as pyramid - using n/(pow(n,i)
    for(int i = 0; i <= log2(n); i++){
        B[i] = (int*)malloc(sizeof(int)*(n/(pow(n,i))));
    }
    //Copy array A to level 0 of array B
    #pragma omp parallel for
    for (int i = 0; i <= n - 1; i++){
        B[0][i] = A[i];
    }
    //Adding each two elements of level 0 to upper level
    for (int h = 1; h <= log2(n); h++){
        int upperbound = (int)((n/pow(2,h))-1);
        #pragma omp parallel for 
        for (int i = 0; i <= upperbound; i++){
            B[h][i] = B[h-1][2*i] + B[h-1][(2*i)+1];
        }
    }

    //Down-sweep
    int *C[(int)(log2(n)+1)]; //array C to hold down-sweep part
    //Initialize as pyramid - using n/(pow(n,i)
    for(int i = 0; i <= log2(n); i++){
        C[i] = (int*)malloc(sizeof(int)*(n/(pow(n,i))));
    }
    //Initialize top level of C to 0
    C[(int)log2(n)][0] = 0;

    //Array C - one level down from top, left child = parent, right child = parent + left child of array B
    for(int h = (int)(log2(n)-1); h >= 0; h--){
        int upperbound = (int)((n/pow(2,h))-1);
        #pragma opm parallel for
        for(int i = 0; i <= upperbound; i++){
            if (i % 2 == 0)
                C[h][i] = C[h+1][i/2];
            else
                C[h][i] = C[h+1][(i-1)/2] + B[h][i-1];    
        }
    }

    //Perform prefix-sum and write it back to array A
    #pragma omp parallel for
    for (int i = 0; i < n; i++){
        A[i] = A[i] + C[0][i];
    }

    //Print out the result for testing purpose
    printf("Prefix-sum Array A = [");
    for (int i = 0; i < n; i++){
        if (i == n-1)
            printf("%d", A[i]);
        else
            printf("%d, ", A[i]);
    }
    printf("]\n");

    //Clean up array B
    for(int i = 0; i <= log2(n); i++){
        free(B[i]);
        free(C[i]);
    }

    return 0;
}
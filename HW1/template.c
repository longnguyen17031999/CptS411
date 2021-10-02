/*
 * template.c
 * Copyright (C) 2021 Aravind SUKUMARAN RAJAM (asr) <aravind_sr@outlook.com>
 *
 * Distributed under terms of the GNU LGPL3 license.
 */

#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <assert.h>


int main()
{
    int N = 100;

    int *A = (int*) malloc (N * N * N * sizeof(int)) ;
    int *B = (int*) malloc (N * N * N * sizeof(int)) ;
    int *C = (int*) malloc (N * N * N * sizeof(int)) ;

    assert(A!=NULL && B!=NULL && C!=NULL);

    //Initialization
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            for (int k = 0; k < N; ++k)
            {
                A[i * N * N + j * N + k] = 1;
                B[i * N * N + j * N + k] = 3;
                C[i * N * N + j * N + k] = 0;
            }
        }
    }



    // computation - Optimize this part
    // timer start
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            for (int k = 0; k < N; ++k)
            {
                C[i * N * N + j * N + k] = A[i * N * N + j * N + k] + B[i * N * N + j * N + k];
            }
        }
    }
    // timer end

    //print -- Since the output will be too long, just print the sum of all elements
    int s=0;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            for (int k = 0; k < N; ++k)
            {
                s += C[i * N * N + j * N + k];
            }
        }
    }

    printf("final sum %d\n", s);


}

#include <stdio.h>
#include <omp.h>
#include <assert.h>
#include <stdlib.h>

int main()
{
    int N = 100;

    int *A = (int*) malloc (N * N * N * sizeof(int)) ;
    int *B = (int*) malloc (N * N * N * sizeof(int)) ;
    int *C = (int*) malloc (N * N * N * sizeof(int)) ;

    assert(A!=NULL && B!=NULL && C!=NULL);

    double start, end, time_in_s;

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

    // 1 threads
    omp_set_num_threads(1);
    // timer start
    start = omp_get_wtime();

    // Parallel region
    #pragma omp parallel for
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
    end = omp_get_wtime();
    time_in_s = end - start;
    printf("\n\nTime taken for 1 thread is %lfs\n", time_in_s);

    // 2 threads
    omp_set_num_threads(2);
    // timer start
    start = omp_get_wtime();

    // Parallel region
    #pragma omp parallel for
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
    end = omp_get_wtime();
    time_in_s = end - start;
    printf("\n\nTime taken for 2 threads is %lfs\n", time_in_s);

    omp_set_num_threads(4);
    // timer start
    start = omp_get_wtime();

    // Parallel region
    #pragma omp parallel for
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
    end = omp_get_wtime();
    time_in_s = end - start;
    printf("\n\nTime taken for 4 threads is %lfs\n", time_in_s);

    omp_set_num_threads(8);
    // timer start
    start = omp_get_wtime();

    // Parallel region
    #pragma omp parallel for
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
    end = omp_get_wtime();
    time_in_s = end - start;
    printf("\n\nTime taken for 8 threads is %lfs\n", time_in_s);

    omp_set_num_threads(16);
    // timer start
    start = omp_get_wtime();

    // Parallel region
    #pragma omp parallel for
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
    end = omp_get_wtime();
    time_in_s = end - start;
    printf("\n\nTime taken for 16 threads is %lfs\n", time_in_s);

    omp_set_num_threads(32);
    // timer start
    start = omp_get_wtime();

    // Parallel region
    #pragma omp parallel for
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
    end = omp_get_wtime();
    time_in_s = end - start;
    printf("\n\nTime taken for 32 threads is %lfs\n", time_in_s);

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
    return 0;
}
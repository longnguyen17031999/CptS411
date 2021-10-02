/*
 * timer.c
 * Copyright (C) 2021 Aravind SUKUMARAN RAJAM (asr) <aravind_sr@outlook.com>
 *
 * Distributed under terms of the GNU LGPL3 license.
 */

#include <stdio.h>
#include <omp.h>

int main()
{


    double start = omp_get_wtime();
    //timed code
    {
        #pragma omp parallel for
        for (int i = 0; i < 100; ++i)
        {
		printf("Hello from thread %d working on iteration %d\n", omp_get_thread_num(), i);

        }

    }
    double end = omp_get_wtime();
    double time_in_s = end - start;
    printf("\n\nTime taken is %lfs\n", time_in_s);

}

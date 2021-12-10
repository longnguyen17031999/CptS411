/*
    Please include this file with other Homework 5 files
    Since it will utilize functions given in those files
    Parallel strategy used: divide work by mat.nnz
    Please include slurm_submit.sh - and modify its .mtx file argument to compile the data needed

    To run this:
    mpic++ HW5.cpp mm_helper.cpp -o myprogram
    sbatch slurm_submit.sh
    cat 411_******.log (****** = id of the job returned)
*/

#include "sparse_representation.hpp"
#include "mm_helper.hpp"
#include <iostream>
#include <mpi.h> 
#include <math.h>

int world_rank;
int world_size;

void init_vec(double *a, unsigned int n, double offset) {
    for (unsigned int i = 0; i < n; ++i) {
        a[i ]  = i  + offset; // you can use rand here
    }
}

void init_vec_out(double* a, unsigned int n){
    for (unsigned int i = 0; i < n; ++i) {
        a[i] = 0; 
    }
}

void print_vec(double *a, unsigned int n) {
    for (unsigned int i = 0; i < n; ++i) {
        std::cout << a[i]   << ' ';
    }
    std::cout << '\n';
}

void print_COO(COO &mat) {
    //    **** This is sequential code ****
    for (unsigned int r = 0; r < mat.nnz; ++r) {
            unsigned int col_id = mat.col_id[r];
            unsigned int row_id = mat.row_id[r];
            double val = mat.values[r];
            std::cout << row_id << ' ' << col_id << ' ' <<  val << '\n';
    }
}

void coo_spmv(COO &mat, double * v_in, double * v_out) {
    /*
        **** This is sequential code ****
        for (unsigned int r = 0; r < mat.nnz; ++r) {
            unsigned int col_id = mat.col_id[r];
            unsigned int row_id = mat.row_id[r];
            double val = mat.values[r];
            v_out[row_id] += val * v_in[col_id];
        }
    */

    // This is MPI code

    int workload = ceil((float)(mat.nnz) / world_size);

    for(int i = 0; i < workload; i++){
        if(i + (world_rank * workload) < mat.nnz){
            unsigned int col_id = mat.col_id[i + (world_rank * workload)];
            unsigned int row_id = mat.row_id[i + (world_rank * workload)];
            double val = mat.values[i + (world_rank * workload)];                 
            v_out[row_id] += val * v_in[col_id];
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
}   

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    if(argc < 2) {
        std::cerr << "usage ./exec inputfile" << std::endl;
        exit(-1);
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    COO mat = read_matrix_market_to_COO(argv[1]);  

    std::cout << mat.nrows << ' ' << mat.ncols << ' ' << mat.nnz << '\n';

    double *v_in = (double*)malloc(mat.ncols * sizeof(double));
    double *v_out = (double*)malloc(mat.nrows * sizeof(double));
    double *global_v_out = (double*)malloc(mat.nrows * sizeof(double));
    
    init_vec(v_in, mat.ncols, 1.0);
    init_vec_out(v_out, mat.nrows);

    coo_spmv(mat, v_in, v_out);
    MPI_Reduce(v_out, global_v_out, mat.nrows, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if (world_rank == 0)
        print_vec(global_v_out, mat.nrows);

    free(mat.row_id);
    free(mat.col_id);
    free(mat.values);
    
    free(v_in);
    free(v_out);
    free(global_v_out);

    MPI_Finalize();
    return EXIT_SUCCESS;
}
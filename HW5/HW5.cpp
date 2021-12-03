/*
    Please include this file with other Homework 5 files
    Since it will utilize functions given in those files
*/

#include "sparse_representation.hpp"
#include "mm_helper.hpp"
#include <iostream>

void init_vec(double *a, unsigned int n, double offset) {
    for (unsigned int i = 0; i < n; ++i) {
        a[i ]  = i  + offset; // you can use rand here
    }
}

void print_vec(double *a, unsigned int n) {
    for (unsigned int i = 0; i < n; ++i) {
        std::cout << a[i]   << ' ';
    }
    std::cout << '\n';
}

void print_COO(COO &mat) {
    for (unsigned int r = 0; r < mat.nnz; ++r) {
            unsigned int col_id = mat.col_id[r];
            unsigned int row_id = mat.row_id[r];
            double val = mat.values[r];
            std::cout << row_id << ' ' << col_id << ' ' <<  val << '\n';
    }
}

void coo_spmv(COO &mat, double * v_in, double * v_out) {
    for (unsigned int r = 0; r < mat.nnz; ++r) {
        unsigned int col_id = mat.col_id[r];
        unsigned int row_id = mat.row_id[r];
        double val = mat.values[r];
        v_out[row_id] += val * v_in[col_id];
    }
}

int main(int argc, char *argv[]) {
    if(argc < 2) {
        std::cerr << "usage ./exec inputfile" << std::endl;
        exit(-1);
    }

    COO mat = read_matrix_market_to_COO(argv[1]);  //CSC and CSR are also available. check mm_helper.hpp to see the functions
    print_COO(mat);
    std::cout << mat.nrows << ' ' << mat.ncols << ' ' << mat.nnz << '\n';

    double *v_in = (double*)malloc(mat.ncols * sizeof(double));
    double *v_out = (double*)malloc(mat.nrows * sizeof(double));

    init_vec(v_in, mat.ncols, 1.0);
    //print_dmat(dmat_in, mat.ncols);
    print_vec(v_in, mat.ncols);

    coo_spmv(mat, v_in, v_out);
    //print_dmat(dmat_out, mat.nrows);
    print_vec(v_out, mat.nrows);

    free(mat.row_id);
    free(mat.col_id);
    free(mat.values);
    return 0;
}
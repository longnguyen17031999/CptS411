/*
 * main.cpp
 * Copyright (C) 2020
 *  Aravind SUKUMARAN RAJAM (asr) <aravind_sr@outlook.com>
 *
 * Distributed under terms of the GNU LGPL3 license.
 */

#include "mm_helper.hpp"
#include "sparse_representation.hpp"
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

void print_CSR(CSR &mat) {
    for (unsigned int r = 0; r < mat.nrows; ++r) {
        unsigned int row_start = mat.row_indx[r];
        unsigned int row_end = mat.row_indx[r + 1];
        for (unsigned int j = row_start; j < row_end; ++j) {
            unsigned int col_id = mat.col_id[j];
            double val = mat.values[j];

            std::cout << r << ' ' << col_id << ' ' <<  val << '\n';
        }
    }
}

void csr_spmv(CSR &mat, double * v_in, double * v_out) {
    for (unsigned int r = 0; r < mat.nrows; ++r) {
        unsigned int row_start = mat.row_indx[r];
        unsigned int row_end = mat.row_indx[r + 1];

        v_out[r] = 0;

        for (unsigned int j = row_start; j < row_end; ++j) {
            unsigned int col_id = mat.col_id[j];
            double val = mat.values[j];
            v_out[r] += val * v_in[col_id];
        }

    }
}

int main(int argc, char *argv[]) {
    if(argc < 2) {
        std::cerr << "usage ./exec inputfile" << std::endl;
        exit(-1);
    }

    CSR mat = read_matrix_market_to_CSR(argv[1]);  //CSC and COO are also available. check mm_helper.hpp to see the functions
    //print_CSR(mat);
    std::cout << mat.nrows << ' ' << mat.ncols << ' ' << mat.nnz << '\n';

    double *v_in = (double*)malloc(mat.ncols * sizeof(double));
    double *v_out = (double*)malloc(mat.nrows * sizeof(double));

    init_vec(v_in, mat.ncols, 1.0);
    //print_dmat(dmat_in, mat.ncols);

    csr_spmv(mat, v_in, v_out);

    //print_dmat(dmat_out, mat.nrows);


    free(mat.row_indx);
    free(mat.col_id);
    free(mat.values);
    return 0;
}

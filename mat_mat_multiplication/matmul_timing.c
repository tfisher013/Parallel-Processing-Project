#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "../util/parse_matrix_file/read_matrix_file.h"
#include "../util/format_datatypes/COO.h"
#include "../util/format_datatypes/CSR.h"
#include "../util/format_datatypes/CSC.h"

void matmat(int *rows, int *cols, double *vals, int rank, int dim, int nnz){

    int proc_offset_A, proc_offset_B;
    if(rank == 0){
        proc_offset_A = 0;
        proc_offset_B = 0;
    }
    else{
        proc_offset_A = n/2;
        proc_offset_B = n/2;
    }

    int non_zero_val_counter = 0;
    for(int i = 0; i<2; i++){
        for(int row = proc_offset_A, row < proc_offset_A+ n/2; row++){
            for(int col = proc_offset_B, col < proc_offset_B + n/2 ; col++){
                double tmp;
                int A_val_index, B_val_index;
                for(int j=0; j < n ; j++){
                    for(int k=0; k < coo_A.nnz; k++){
                        if (coo_A.rows[k] = row && coo_A.cols[k] == j){
                            A_val_index = k;
                            if(B_val_index != 0){break;}
                        }
                        if (coo_B.rows[k]==j && coo_B.cols[k] = col){
                            B_val_index = k;
                            if(A_val_index != 0) { break;}
                        }
                    }

                    if(A_val_index && B_val_index){
                        tmp += coo_A.vals[A_val_index] * coo_B.vals[B_val_index];
                    }
                }
                coo_C.cols[non_zero_val_counter] = col;
                coo_C.rows[non_zero_val_counter] = row;
                coo_C.vals[non_zero_val_counter] = tmp;
                non_zero_val_counter++;
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);
        if(rank ==0){
            proc_offset_B = n/2;
        }
        else{
            proc_offset_B = 0;
        }
    }
}


int main(int argc, &argv){
    MPI_Init(&argc,&argv);
    int rank, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

     char *matrices_100[] = {"../util/matrix_generators/matrices_100/dimension_100_nonzeros_100.mtx",
                          "../util/matrix_generators/matrices_100/dimension_100_nonzeros_1090.mtx",
                          "../util/matrix_generators/matrices_100/dimension_100_nonzeros_2080.mtx",
                          "../util/matrix_generators/matrices_100/dimension_100_nonzeros_3070.mtx",
                          "../util/matrix_generators/matrices_100/dimension_100_nonzeros_4060.mtx",
                          "../util/matrix_generators/matrices_100/dimension_100_nonzeros_5050.mtx",
                          "../util/matrix_generators/matrices_100/dimension_100_nonzeros_6040.mtx",
                          "../util/matrix_generators/matrices_100/dimension_100_nonzeros_7030.mtx",
                          "../util/matrix_generators/matrices_100/dimension_100_nonzeros_8020.mtx",
                          "../util/matrix_generators/matrices_100/dimension_100_nonzeros_9010.mtx"};

  char *matrices_1000[] = {
                        "../matrices/standardized_matrices/dimension_1000_nonzeros_1000.mtx",
                        "../matrices/standardized_matrices/dimension_1000_nonzeros_100900.mtx",
                        "../matrices/standardized_matrices/dimension_1000_nonzeros_200800.mtx",
                        "../matrices/standardized_matrices/dimension_1000_nonzeros_300700.mtx",
                        "../matrices/standardized_matrices/dimension_1000_nonzeros_400600.mtx",
                        "../matrices/standardized_matrices/dimension_1000_nonzeros_500499.mtx",
                        "../matrices/standardized_matrices/dimension_1000_nonzeros_600400.mtx",
                        "../matrices/standardized_matrices/dimension_1000_nonzeros_700300.mtx",
                        "../matrices/standardized_matrices/dimension_1000_nonzeros_800200.mtx",
                        "../matrices/standardized_matrices/dimension_1000_nonzeros_900100.mtx"};


for (int i = 0; i < 10; i++)
  {
    int dim = getStandardMatrixDimension(matrices_100[i]);
    double *matrix = calloc(dim * dim, sizeof(double));
    getStandardMatrix(matrices_100[i], dim, matrix);

    // perform CSC pingpong timing
    CSC csc;
    convertToCSC(&csc, dim, dim, matrix);
    double density = csc.nnz / ((double) dim * dim);
    if(rank == 0){
        printf("Rank %d has Matrix of Dimension %d x %d and density %e and nnz %d and name %s\n", rank, dim, dim, density, csc.nnz, matrices_100[i]);
    }
    matmat(csc.values, csc.colptrs, csc.rows, csc.nnz, numpong, rank, dim);
    freeCSC(&csc);

    MPI_Barrier(MPI_COMM_WORLD);

    // perform COO pingpong timing
    COO coo;
    convertToCOO(&coo, dim, dim, matrix);
    matmat(coo.values, coo.cols, coo.rows, coo.nnz, numpong, rank, dim);
    freeCOO(&coo);

    MPI_Barrier(MPI_COMM_WORLD);

    // perform CSR pingpong timing
    CSR csr;
    convertToCSR(&csr, dim, dim, matrix);
    matmat(csr.values, csr.cols, csr.rowPtrs, csr.nnz, numpong, rank, dim);
    freeCSR(&csr);
    free(matrix);

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0)
      printf("\n");
  }

  MPI_Finalize();
}

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "../util/parse_matrix_file/read_matrix_file.h"
#include "../util/format_datatypes/COO.h"
#include "../util/format_datatypes/CSR.h"
#include "../util/format_datatypes/CSC.h"

void matmat(COO *coo_A, COO *coo_B, COO *coo_C, int rank, int dim){

    int proc_offset_A, proc_offset_B;
    if(rank == 0){
        proc_offset_A = 0;
        proc_offset_B = 0;
    }
    else{
        proc_offset_A = dim/2;
        proc_offset_B = dim/2;
    }

    int non_zero_val_counter = 0;
    for(int i = 0; i<2; i++){
        for(int row = proc_offset_A; row < proc_offset_A+ dim/2; row++){
            for(int col = proc_offset_B; col < proc_offset_B + dim/2 ; col++){
                double tmp;
                int A_val_index, B_val_index;
                for(int j=0; j < dim ; j++){
                    for(int k=0; k < coo_A->nnz; k++){
                        if (coo_A->rows[k] = row && coo_A->cols[k] == j){
                            A_val_index = k;
                            if(B_val_index != 0){break;}
                        }
                        if (coo_B->rows[k]==j && coo_B->cols[k] == col){
                            B_val_index = k;
                            if(A_val_index != 0) { break;}
                        }
                    }

                    if(A_val_index && B_val_index){
                        tmp += coo_A->values[A_val_index] * coo_B->values[B_val_index];
			printf("temp value is %f\n",tmp);
                    }
                }
                coo_C->cols[non_zero_val_counter] = col;
                coo_C->rows[non_zero_val_counter] = row;
                coo_C->values[non_zero_val_counter] = tmp;
                non_zero_val_counter++;
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);
        if(rank ==0){
            proc_offset_B = dim/2;
        }
        else{
            proc_offset_B = 0;
        }
    }
}


int main(int argc, char *argv[]){
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


for (int i = 0; i < 10; i=i+2)
  {
    int dim = getStandardMatrixDimension(matrices_100[i]);
    double *matrix_A = calloc(dim * dim, sizeof(double));
    double *matrix_B = calloc(dim * dim, sizeof(double));
    double *matrix_C = calloc(dim * dim, sizeof(double));
    getStandardMatrix(matrices_100[i], dim, matrix_A);
//Might need a barrier here
    getStandardMatrix(matrices_100[i+1], dim, matrix_B);
//Might need a barrier here


    // perform COO matmul timing
    COO coo_A;
    COO coo_B;
    COO coo_C;

    convertToCOO(&coo_A, dim, dim, matrix_A);
    convertToCOO(&coo_B, dim, dim, matrix_B);
    convertToCOO(&coo_C, dim, dim, matrix_C);
    matmat(&coo_A, &coo_B, &coo_C, rank, dim);
    freeCOO(&coo_A);
    freeCOO(&coo_B);
    freeCOO(&coo_C);

    MPI_Barrier(MPI_COMM_WORLD);

/*
    // perform CSC matmul timing
    CSC csc_A;
    CSC csc_B;
    convertToCSC(&csc_A, dim, dim, matrix_A);
    convertToCSC(&csc_B, dim, dim, matrix_B);
    
    double density = csc.nnz / ((double) dim * dim);
    if(rank == 0){
        printf("Rank %d has Matrix A of Dimension %d x %d and density %e and nnz %d and name %s\n", rank, dim, dim, density, csc.nnz, matrices_100[i]);
        printf("Rank %d has Matrix B of Dimension %d x %d and density %e and nnz %d and name %s\n", rank, dim, dim, density, csc.nnz, matrices_100[i+1]);

    }
    matmat(CSC csc_A, CSC csc_B, rank, dim);
    freeCSC(&csc);

    MPI_Barrier(MPI_COMM_WORLD);

    // perform CSR pingpong timing
    CSR csr_A;
    CSR csr_B;
    convertToCSR(&csr_A, dim, dim, matrix_A);
    convertToCSR(&csr_B, dim, dim, matrix_B);
    matmat(&csr_A,&csr_B, rank, dim);
    freeCSR(&csr);
    free(matrix);
*/
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0)
      printf("\n");
  }

  MPI_Finalize();
}

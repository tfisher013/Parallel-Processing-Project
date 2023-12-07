#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../util/parse_matrix_file/read_matrix_file.h"
#include "../util/format_datatypes/COO.h"
#include "../util/format_datatypes/CSR.h"

void matmatCSR(CSR *csr_A, CSR *csr_B, int dim){

    int non_zero_val_counter = 0;
        for(int row = 0; row < dim; row++){
            for(int col = 0; col < dim; col++){
                double tmp = 0;
                for(int j=0; j < dim ; j++){

                    int A_val_index = -1; 
                    int B_val_index = -1;

                    // satifies finding nnz in A at row, col
                    for(int k = csr_A->rowPtrs[row]; k < csr_A->rowPtrs[row+1]; k++){
                        if(csr_A->cols[k] == j){
                            A_val_index = k;
                            break;
                        }
                    }

                    // do the same for B
                    for(int k = 0; k < csr_A->nnz; k++){
                        if(csr_A->cols[k] == col){
                            if(k >= csr_A->rowPtrs[j] && k < csr_A->rowPtrs[j+1]){
                                B_val_index = k;
                                break;
                            }
                        }
                    }

                    if(A_val_index >= 0 && B_val_index >= 0){
                        tmp += csr_A->values[A_val_index] * csr_B->values[B_val_index];
                    }
                }
               // printf("The value at row %d and col %d is %f\n", row, col, tmp);
                non_zero_val_counter++;
            }
        }
}

void matmatCOO(COO *coo_A, COO *coo_B, int dim){

    int non_zero_val_counter = 0;
        for(int row = 0; row < dim; row++){
            for(int col = 0; col < dim; col++){
                double tmp = 0;
                for(int j=0; j < dim ; j++){
                    int A_val_index = -1; 
                    int B_val_index = -1;
                    for(int k=0; k < coo_A->nnz; k++){
                        if (coo_A->rows[k] == row && coo_A->cols[k] == j){
                            A_val_index = k;
                            break;
                        }
                    }
                    for(int k=0; k < coo_B->nnz; k++){
                        if (coo_B->rows[k]==j && coo_B->cols[k] == col){
                            B_val_index = k;
                            break;
                        }
                    }

                    if(A_val_index >= 0 && B_val_index >= 0){
                        tmp += coo_A->values[A_val_index] * coo_B->values[B_val_index];
                    }
                }
               // printf("The value at row %d and col %d is %f\n", row, col, tmp);
                non_zero_val_counter++;
            }
        }
}


int main(int argc, char *argv[]){

    char *matrices_100[] = {"../matrices/standardized_matrices/dimension_100_nonzeros_100.mtx",
                        "../matrices/standardized_matrices/dimension_100_nonzeros_1090.mtx",
                        "../matrices/standardized_matrices/dimension_100_nonzeros_2080.mtx",
                        "../matrices/standardized_matrices/dimension_100_nonzeros_3070.mtx",
                        "../matrices/standardized_matrices/dimension_100_nonzeros_4060.mtx",
                        "../matrices/standardized_matrices/dimension_100_nonzeros_5050.mtx",
                        "../matrices/standardized_matrices/dimension_100_nonzeros_6040.mtx",
                        "../matrices/standardized_matrices/dimension_100_nonzeros_7030.mtx",
                        "../matrices/standardized_matrices/dimension_100_nonzeros_8020.mtx",
                        "../matrices/standardized_matrices/dimension_100_nonzeros_9010.mtx"};
                        

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

   for(int i =0; i<10; i++){
    char *filename = matrices_100[i];
    int dim = getStandardMatrixDimension(filename);
    double *matrix_A = calloc(dim * dim, sizeof(double));
    //double *matrix_B = calloc(dim * dim, sizeof(double));
    getStandardMatrix(filename, dim, matrix_A);
    //getStandardMatrix(filename, dim, matrix_B);

    // perform COO matmul timing
    COO coo_A;
    CSR csr_A;
   // COO coo_B;

    convertToCOO(&coo_A, dim, dim, matrix_A);
    convertToCSR(&csr_A, dim, dim, matrix_A);
    //printCOO(&coo_A);
    //convertToCOO(&coo_B, dim, dim, matrix_B);
    clock_t start_time, end_time;
    double cpu_time_used;
    double total_cpu_time_used = 0;
    double average_cpu_time;


    for (int iteration = 0; iteration < 100; iteration++) {
        start_time = clock();

        matmatCSR(&csr_A, &csr_A, dim);

        end_time = clock();
        cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
        total_cpu_time_used += cpu_time_used;

    }
    average_cpu_time = total_cpu_time_used / 100.0;
    printf("Average time taken by matmatCSR over 100 iterations: %f seconds\n", average_cpu_time);

    for (int iteration = 0; iteration < 100; iteration++) {
        start_time = clock();

        matmatCOO(&coo_A, &coo_A, dim);
        
        end_time = clock();
        cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
        total_cpu_time_used += cpu_time_used;

    }
    average_cpu_time = total_cpu_time_used / 100.0;
    printf("Average time taken by matmatCOO over 100 iterations: %f seconds\n", average_cpu_time);

    freeCSR(&csr_A);
    freeCOO(&coo_A);


  return 0;
}
}

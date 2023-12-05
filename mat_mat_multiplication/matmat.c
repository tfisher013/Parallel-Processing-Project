#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../util/parse_matrix_file/read_matrix_file.h"
#include "../../util/format_datatypes/COO.h"
#include "../../util/format_datatypes/CSR.h"

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
                printf("The value at row %d and col %d is %f\n", row, col, tmp);
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
                printf("The value at row %d and col %d is %f\n", row, col, tmp);
                non_zero_val_counter++;
            }
        }
}


int main(int argc, char *argv[]){

    char *filename = "identity2.mtx";
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
    matmatCSR(&csr_A, &csr_A, dim);
    freeCSR(&csr_A);
    //freeCOO(&coo_B);


  return 0;
}

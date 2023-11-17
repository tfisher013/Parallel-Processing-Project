#include <stdio.h>
#include <stdlib.h>
#include "CSR.h"

/*
int main() {
    // Example usage
    int n = 3, m = 3; // dimensions of the matrix
    int **mat = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        mat[i] = malloc(m * sizeof(int));
        // Initialize mat[i] here
    }

    mat[0][0] = 1; mat[0][1] = 0; mat[0][2] = 2;
    mat[1][0] = 0; mat[1][1] = 3; mat[1][2] = 0;
    mat[2][0] = 4; mat[2][1] = 0; mat[2][2] = 5;

    CSR csr;
    convertToCSR(&csr, mat, n, m);
    printCSR(&csr);

    // Free memory
    freeCSR(&csr);
    for (int i = 0; i < n; i++) {
        free(mat[i]);
    }
    free(mat);

    return 0;
}
*/


void convertToCSR(CSR *csr, int n, int m, double mat[n][m]) {
    int nnz = 0;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            if(mat[i][j] != 0)
                nnz++;
        }
    }

    csr->values = malloc(nnz * sizeof(double));
    csr->cols = malloc(nnz * sizeof(double));
    csr->rowPtrs = malloc((m + 1) *sizeof(double));
    csr->nnz = nnz;
    csr->numRowPtrs = 0;

    int k = 0;
    for(int i = 0; i < n; i++) {
        csr->rowPtrs[i] = k;
        csr->numRowPtrs++;
        for(int j = 0; j < m; j++) {
            if(mat[i][j] != 0) {
                csr->values[k] = mat[i][j];
                csr->cols[k] = j;
                k++;
            }
        }
    }
    csr->rowPtrs[m] = k;
}

void printCSR(CSR *csr) {
    printf("Values: ");
    for(int i = 0; i < csr->nnz; i++) {
        printf("%f ", csr->values[i]);
    }
    printf("\n");

    printf("Row Pointers: ");
    for(int i = 0; i <= csr->numRowPtrs; i++) {
        printf("%d ", csr->rowPtrs[i]);
    }
    printf("\n");

    printf("cols: ");
    for(int i = 0; i < csr->nnz; i++) {
        printf("%d ", csr->cols[i]);
    }
    printf("\n");
}


void freeCSR(CSR *csr) {
    free(csr->values);
    free(csr->rowPtrs);
    free(csr->cols);
}

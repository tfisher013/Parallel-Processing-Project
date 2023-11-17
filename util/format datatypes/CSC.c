#include <stdio.h>
#include <stdlib.h>
#include "CSC.h"

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

    CSC coo;
    convertToCSC(&coo, mat, n, m);
    printCSC(&coo);

    // Free memory
    freeCSC(&coo);
    for (int i = 0; i < n; i++) {
        free(mat[i]);
    }
    free(mat);

    return 0;
}
*/

void convertToCSC(CSC *csc, int n, int m, double mat[n][m]) {
    int nnz = 0;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            if(mat[i][j] != 0)
                nnz++;
        }
    }

    csc->values = malloc(nnz * sizeof(double));
    csc->colptrs = malloc((m + 1) * sizeof(double));
    csc->rows = malloc(nnz * sizeof(double));
    csc->nnz = nnz;
    csc->numColPtrs = 0;

    int k = 0;
    for(int j = 0; j < m; j++) {
        csc->colptrs[j] = k;
        csc->numColPtrs++;
        for(int i = 0; i < n; i++) {
            if(mat[i][j] != 0) {
                csc->values[k] = mat[i][j];
                csc->rows[k] = i;
                k++;
            }
        }
    }
    csc->colptrs[m] = k;
}

void printCSC(CSC *csc) {
    printf("Values: ");
    for(int i = 0; i < csc->nnz; i++) {
        printf("%f ", csc->values[i]);
    }
    printf("\n");

    printf("Column Pointers: ");
    for(int i = 0; i <= csc->numColPtrs; i++) 
    {
        printf("%d ", csc->colptrs[i]);
    }
    printf("\n");

    printf("Rows: ");
    for(int i = 0; i < csc->nnz; i++) {
        printf("%d ", csc->rows[i]);
    }
    printf("\n");
}


void freeCSC(CSC *csc) 
{
    free(csc->values);
    free(csc->colptrs);
    free(csc->rows);
}
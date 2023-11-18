#include <stdio.h>
#include <stdlib.h>
#include "COO.h"

// int main() {
//     // Example usage
//     int n = 3, m = 3; // dimensions of the matrix
//     double **mat = malloc(n * sizeof(double *));
//     for (int i = 0; i < n; i++) {
//         mat[i] = malloc(m * sizeof(double));
//         // Initialize mat[i] here
//     }
    
//     mat[0][0] = 1; mat[0][1] = 0; mat[0][2] = 2;
//     mat[1][0] = 0; mat[1][1] = 3; mat[1][2] = 0;
//     mat[2][0] = 4; mat[2][1] = 0; mat[2][2] = 5;

//     COO coo;
//     convertToCOO(&coo, n, m, mat);
//     printCOO(&coo);

//     // Free memory
//     freeCOO(&coo);
//     for (int i = 0; i < n; i++) {
//         free(mat[i]);
//     }
//     free(mat);

//     return 0;
// }

void convertToCOO(COO *coo, int n, int m, double mat[n][m]) {
    int nnz = 0;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            if(mat[i][j] != 0)
                nnz++;
        }
    }

    coo->values = malloc(nnz * sizeof(double));
    coo->cols = malloc(nnz * sizeof(double));
    coo->rows = malloc(nnz * sizeof(double));
    coo->nnz = nnz;

    int k = 0;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            if(mat[i][j] != 0) {
                coo->values[k] = mat[i][j];
                coo->cols[k] = j;
                coo->rows[k] = i;
                k++;
            }
        }
    }
}

void printCOO(COO *coo) {
    printf("Values: ");
    for(int i = 0; i < coo->nnz; i++) {
        printf("%f ", coo->values[i]);
    }
    printf("\n");

    printf("Columns: ");
    for(int i = 0; i < coo->nnz; i++) {
        printf("%d ", coo->cols[i]);
    }
    printf("\n");

    printf("Rows: ");
    for(int i = 0; i < coo->nnz; i++) {
        printf("%d ", coo->rows[i]);
    }
    printf("\n");
}

void freeCOO(COO *coo) {
    free(coo->values);
    free(coo->cols);
    free(coo->rows);
}
#include <stdio.h>
#include <stdlib.h>
#include "CSC.h"

void convertToCSC(CSC *csc, int n, int m, double *mat)
{
    int nnz = 0;
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            if (mat[i * n + j] != 0)
                nnz++;
        }
    }

    csc->values = malloc(nnz * sizeof(double));
    csc->colptrs = malloc((m + 1) * sizeof(int));
    csc->rows = malloc(nnz * sizeof(int));
    csc->nnz = nnz;
    csc->numColPtrs = 0;

    int k = 0;
    for (int j = 1; j <= m; j++)
    {
        csc->colptrs[j] = k;
        csc->numColPtrs++;
        for (int i = 1; i <= n; i++)
        {
            if (mat[i * n + j] != 0)
            {
                csc->values[k] = mat[i * n + j];
                csc->rows[k] = i;
                k++;
            }
        }
    }
    csc->colptrs[m] = k;
}

void printCSC(CSC *csc)
{
    printf("Values: ");
    for (int i = 0; i < csc->nnz; i++)
    {
        printf("%f ", csc->values[i]);
    }
    printf("\n");

    printf("Column Pointers: ");
    for (int i = 0; i <= csc->numColPtrs; i++)
    {
        printf("%d ", csc->colptrs[i]);
    }
    printf("\n");

    printf("Rows: ");
    for (int i = 0; i < csc->nnz; i++)
    {
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

#include <stdio.h>
#include <stdlib.h>
#include "COO.h"

void convertToCOO(COO *coo, int n, int m, double *mat)
{
    int nnz = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (mat[i * n + j] != 0)
                nnz++;
        }
    }

    coo->values = malloc(nnz * sizeof(double));
    coo->cols = malloc(nnz * sizeof(double));
    coo->rows = malloc(nnz * sizeof(double));
    coo->nnz = nnz;

    int k = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (mat[i * n + j] != 0)
            {
                coo->values[k] = mat[i * n + j];
                coo->cols[k] = j;
                coo->rows[k] = i;
                k++;
            }
        }
    }
}

void printCOO(COO *coo)
{
    printf("Values: ");
    for (int i = 0; i < coo->nnz; i++)
    {
        printf("%f ", coo->values[i]);
    }
    printf("\n");

    printf("Columns: ");
    for (int i = 0; i < coo->nnz; i++)
    {
        printf("%d ", coo->cols[i]);
    }
    printf("\n");

    printf("Rows: ");
    for (int i = 0; i < coo->nnz; i++)
    {
        printf("%d ", coo->rows[i]);
    }
    printf("\n");
}

void freeCOO(COO *coo)
{
    free(coo->values);
    free(coo->cols);
    free(coo->rows);
}
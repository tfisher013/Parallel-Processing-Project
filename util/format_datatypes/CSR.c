#include <stdio.h>
#include <stdlib.h>
#include "CSR.h"

void convertToCSR(CSR *csr, int n, int m, double *mat)
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

    csr->values = malloc(nnz * sizeof(double));
    csr->cols = malloc(nnz * sizeof(int));
    csr->rowPtrs = malloc((m + 1) * sizeof(int));
    csr->nnz = nnz;
    csr->numRowPtrs = 0;

    int k = 0;
    for (int i = 1; i <= n; i++)
    {
        csr->rowPtrs[i] = k;
        csr->numRowPtrs++;
        for (int j = 1; j <= m; j++)
        {
            if (mat[i * n + j] != 0)
            {
                csr->values[k] = mat[i * n + j];
                csr->cols[k] = j;
                k++;
            }
        }
    }
    csr->rowPtrs[m] = k;
}

void printCSR(CSR *csr)
{
    printf("Values: ");
    for (int i = 0; i < csr->nnz; i++)
    {
        printf("%f ", csr->values[i]);
    }
    printf("\n");

    printf("Row Pointers: ");
    for (int i = 0; i <= csr->numRowPtrs; i++)
    {
        printf("%d ", csr->rowPtrs[i]);
    }
    printf("\n");

    printf("cols: ");
    for (int i = 0; i < csr->nnz; i++)
    {
        printf("%d ", csr->cols[i]);
    }
    printf("\n");
}

void freeCSR(CSR *csr)
{
    free(csr->values);
    free(csr->rowPtrs);
    free(csr->cols);
}

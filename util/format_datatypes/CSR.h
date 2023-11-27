typedef struct {
    double *values;
    int *cols;
    int *rowPtrs;
    int nnz; // number of non-zero elements
    int numRowPtrs;
} CSR;

void convertToCSR(CSR *csr, int n, int m, double *mat);
void printCSR(CSR *csr);
void freeCSR(CSR *csr);
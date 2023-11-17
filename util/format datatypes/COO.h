typedef struct {
    double *values;
    int *cols;
    int *rows;
    int nnz; // number of non-zero elements
} COO;

void convertToCOO(COO *coo, int n, int m, double mat[n][m]);
void printCOO(COO *coo);
void freeCOO(COO *coo);
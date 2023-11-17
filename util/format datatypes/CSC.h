typedef struct {
  double *values;
  int *colptrs;
  int *rows;
  int nnz; // number of non-zero elements
  int numColPtrs;
} CSC;

void convertToCSC(CSC *csc, int n, int m, double mat[n][m]);
void printCSC(CSC *csc);
void freeCSC(CSC *csc);
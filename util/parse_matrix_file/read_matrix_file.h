/**
 * Returns the dimension of the matrix stored in the provided file.
 * ASSUMPTIONS:
 *  1. matrix is stored in matrix market format (.mtx)
 *  2. matrix is square
 */
int getStandardMatrixDimension(char *fileName);

/**
 * Modifies the supplied 2D array to contain the values in the matrix
 * stored in the provided file.
 * ASSUMPTIONS:
 *  1. matrix is stored in matrix market format (.mtx)
 *  2. matrix is of type double
 */
void getStandardMatrix(char *fileName, int dim, double *matrix);

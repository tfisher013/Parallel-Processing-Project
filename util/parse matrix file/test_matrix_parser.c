#include <stdio.h>
#include "read_matrix_file.h"

/**
 * Demonstration of how to use matrix file parser,
 * in 3 easy steps! :))))
 * 
 * To compile and run this example:
 * gcc test_matrix_parser.c read_file_matrix.c -o test_matrix_parser
 * ./ test_matrix_parser cage3.mtx
 */
int main(int argc, char *argv[])
{
    // 1. get the dimension of the matrix from the file
    // (assumed to be square) by passing file name to
    // the method below
    int dim = getStandardMatrixDimension(argv[1]);

    // 2. intialize a 2D array of correct dimension to hold
    // data
    double matrix[dim][dim];

    // 3. pass matrix to this method where it will be populated
    // with matrix data
    getStandardMatrix(argv[1], dim, matrix);

    // print out results of 2D array
    // --> I wouldn't recommend this for large arrays :|
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            printf("%f ", matrix[i][j]);
        }
        printf("\n");
    }
}
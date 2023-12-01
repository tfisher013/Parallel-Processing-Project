#include <stdio.h>
#include <stdlib.h>
#include "../../util/parse_matrix_file/read_matrix_file.h"
#include "../../util/format_datatypes/COO.h"
#include "../../util/format_datatypes/CSR.h"
#include "../../util/format_datatypes/CSC.h"

/**
 * File which loads a matrix into data structures. Intended to be
 * used while running a memory profiler to compare memory usage of
 * different density matrices.
 */
int main(int argc, char *argv[])
{

    if (argc == 1)
    {
        printf("Please provide a file name parameter. Exiting.\n");
        return 0;
    }

    int dim = getStandardMatrixDimension(argv[1]);
    double *matrix = malloc(dim * dim * sizeof(double));
    getStandardMatrix(argv[1], dim, matrix);

    // convert 2D matrix to COO format and print
    COO coo;
    convertToCOO(&coo, dim, dim, matrix);
    freeCOO(&coo);

    // convert 2D matrix to CSR format and print
    CSR csr;
    convertToCSR(&csr, dim, dim, matrix);
    freeCSR(&csr);

    // convert 2D matrix to CSC format and print
    CSC csc;
    convertToCSC(&csc, dim, dim, matrix);
    freeCSC(&csc);

    free(matrix);

    printf("done\n");
}

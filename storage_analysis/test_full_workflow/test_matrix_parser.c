#include <stdio.h>
#include "../../util/parse_matrix_file/read_matrix_file.h"
#include "../../util/format_datatypes/COO.h"
#include "../../util/format_datatypes/CSR.h"
#include "../../util/format_datatypes/CSC.h"

/**
 * Demonstration of how to use matrix file parser,
 * in 3 easy steps! :))))
 *
 * To compile and run this example:
 * 1. Create object files for this file and all dependencies
 *  gcc -c test_matrix_parser.c --> test_matrix_parser.o
 *  gcc -c ../../util/format_datatypes/COO.c --> COO.o
 *  gcc -c ../../util/format_datatypes/CSR.c --> CSR.o
 *  gcc -c ../../util/format_datatypes/CSC.c --> CSC.o
 *  gcc -c ../../util/parse_matrix_file/read_matrix_file.c --> test_matrix_parser.o
 * 2. Create executable from all linked object files created above
 *  gcc -o executable_name COO.o CSR.o CSC.o read_matrix_file.o test_matrix_parser.o
 * 3. Run executable (don't forget to provide filename arg)
 *  ./executable_name ../../matrices/matrix_file_name
 * 
 */
int main(int argc, char *argv[])
{

    if (argc == 1)
    {
        printf("Please provide a file name parameter. Exiting.\n");
        return 0;
    }

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
    printf("Printing matrix in 2D format:\n");
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            printf("%f ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    // convert 2D matrix to COO format and print
    COO coo;
    convertToCOO(&coo, dim, dim, matrix);
    printf("Printing matrix in COO format:\n");
    printCOO(&coo);
    freeCOO(&coo);

    // convert 2D matrix to CSR format and print
    CSR csr;
    convertToCSR(&csr, dim, dim, matrix);
    printf("Printing matrix in CSR format:\n");
    printCSR(&csr);
    freeCSR(&csr);

    // convert 2D matrix to CSC format and print
    CSC csc;
    convertToCSC(&csc, dim, dim, matrix);
    printf("Printing matrix in CSC format:\n");
    printCSC(&csc);
    freeCSC(&csc);

    // do whatever you want...

}

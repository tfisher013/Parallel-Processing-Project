#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_matrix_file.h"

/**
 * Returns the dimension of the matrix stored in the provided file.
 * ASSUMPTIONS:
 *  1. matrix is stored in matrix market format (.mtx)
 *  2. matrix is square
 */
int getStandardMatrixDimension(char *fileName)
{
    FILE *fptr;

    // Open a file in read mode
    fptr = fopen(fileName, "r");

    // Create array to hold each line
    char fileLine[100];

    int dim = 0;
    while (fgets(fileLine, 100, fptr))
    {
        // Skip commented lines at the top of the file (this seems to
        // be the format for all files from https://www.cise.ufl.edu/
        // research/sparse/matrices/list_by_id.html)
        if (fileLine[0] == '%')
        {
            printf("Ignoring line\n");
        }
        // extract row (assumed to equal col) from first line
        else
        {
            char *token;
            char *rest = fileLine;
            char *p[1];

            while ((token = strtok_r(rest, " ", &rest)))
            {
                p[0] = token;
                break;
            }

            dim = atoi(p[0]);
            break;
        }
    }

    return dim;
}

/**
 * Returns the matrix stored in the provided file as a 2D matrix.
 * ASSUMPTIONS:
 *  1. matrix is stored in matrix market format (.mtx)
 *  2. matrix is of type double
 */
double **getStandardMatrix(char *fileName, double **matrix)
{

    FILE *fptr;

    // Open a file in read mode
    fptr = fopen(fileName, "r");

    // Create array to hold each line
    char fileLine[100];

    int nrows = 0;
    int ncols = 0;
    int nonZeros = 0;
    while (fgets(fileLine, 100, fptr))
    {
        // Skip commented lines at the top of the file (this seems to
        // be the format for all files from https://www.cise.ufl.edu/
        // research/sparse/matrices/list_by_id.html)
        if (fileLine[0] == '%')
        {
            printf("Ignoring line\n");
        }
        // extract row, col, and # nonzeros from first line
        else
        {
            char *token;
            char *rest = fileLine;
            char *p[3];

            int counter = 0;
            while ((token = strtok_r(rest, " ", &rest)))
            {
                p[counter++] = token;
            }

            nrows = atoi(p[0]);
            ncols = atoi(p[1]);
            nonZeros = atoi(p[2]);

            break;
        }
    }

    // intialize a matrix of zeros of correct dimension
    double funcMatrix[nrows][ncols];

    // place nonzeros into correct positions in matrix
    while (fgets(fileLine, 100, fptr))
    {
        char *token;
        char *rest = fileLine;
        char *p[3];
        char *q;

        int counter = 0;
        while ((token = strtok_r(rest, " ", &rest)))
        {
            p[counter++] = token;
        }

        int row = atoi(p[0]);
        int col = atoi(p[1]);
        double val = strtod(p[2], NULL);

        matrix[row][col] = val;
    }

    // Close the file
    fclose(fptr);

    // for (int i = 0; i < nrows; i++)
    // {
    //     double *row[ncols];
    //     for (int j = 0; j < ncols; j++)
    //     {
    //         *(row[j]) = funcMatrix[i][j];
    //     }
    //     *(matrix[i]) = row;
    // }

    return matrix;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_matrix_file.h"

/**
 * Returns the dimension of the matrix stored in the provided file.
 * ASSUMPTIONS:
 *  1. matrix is stored in matrix market format (.mtx)
 *  2. matrix is square
 *
 * Arguments:
 *  char *filename
 *
 * Return Type:
 *  int
 */
int getStandardMatrixDimension(char *filename)
{
    // Open a file in read mode
    FILE *fptr;
    fptr = fopen(filename, "r");

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
        }
        // extract row (assumed to equal col) from first line
        else
        {
            char *token;
            char *rest = fileLine;
            char *p[1];

            // split line with delimiter = " "
            while ((token = strtok_r(rest, " ", &rest)))
            {
                p[0] = token;
                break;
            }

            // store first integer (# rows) on this line
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
 *  2. matrix values are of type double
 *
 * Arguments:
 *  char *filename
 *  int dim
 *  double *matrix
 *
 * Return Type:
 *  void
 */
void getStandardMatrix(char *fileName, int dim, double *matrix)
{
    // Open a file in read mode
    FILE *fptr;
    fptr = fopen(fileName, "r");

    // Create array to hold each line
    char fileLine[100];

    int firstLine = 0;
    while (fgets(fileLine, 100, fptr))
    {
        // Skip commented lines at the top of the file (this seems to
        // be the format for all files from https://www.cise.ufl.edu/
        // research/sparse/matrices/list_by_id.html)
        if (fileLine[0] == '%')
        {
        }
        else if (firstLine == 0)
        {
            // register when we pass the dimension line (data in that
            // line not needed for this method though)
            firstLine = 1;
        }
        else
        {
            char *token;
            char *rest = fileLine;
            const char *p[3];
            char *q;

            int counter = 0;
            while ((token = strtok_r(rest, " ", &rest)))
            {
                p[counter++] = token;
            }

            int row = atoi(p[0]);
            int col = atoi(p[1]);
            double val = strtod(p[2], NULL);

            matrix[(row - 1) * dim + col - 1] = val;
        }
    }

    // Close the file
    fclose(fptr);
}

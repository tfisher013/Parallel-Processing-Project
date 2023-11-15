#include <stdio.h>
#include "read_matrix_file.h"

int main(int argc, char *argv[])
{

    int dim = getStandardMatrixDimension(argv[0]);
    double **emptyMatrix;
    double **matrix = getStandardMatrix(argv[0], emptyMatrix);

    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            printf("%f ", *(&(matrix[0])[j]));
        }
        printf("\n");
    }
}
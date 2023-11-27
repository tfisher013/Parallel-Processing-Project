#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "uniform_random_matrix_generator.h"

/**
 * Populates a square 2D array of type double with values according to the
 * supplied matrix dimension and density. The nonzero elements of the matrix
 * are distributed uniformly throughout the matrix.
 *
 * Arguments:
 *  int dim
 *  double density
 *
 * Return type
 *  double**
 */
// void uniformRandomMatrix(const int dim, const double density, double matrix[][dim])
void uniformRandomMatrix(const int dim, const double density, double *matrix)
{
    // set up random number generator
    // thanks to https://stackoverflow.com/questions/33058848/generate-a-random-double-between-1-and-1
    // for inspiring this implementation <3
    double MIN = 0.0;
    double MAX = 1.0;
    double RANGE = MAX - MIN;
    double div = RAND_MAX / RANGE;
    srand(time(NULL));

    int num_non_zeros = 0;

    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {

            // generate a random number double in [0, 1]; if value is
            // less than density, entry remains 0
            double rand_num = MIN + (rand() / div);

            if (rand_num <= density)
            {
                matrix[i * dim + j] = rand_num;
                num_non_zeros++;
            }
        }
    }

    printf("Matrix of dimension %d with expected density %f has actual density %f.\n", dim, density, num_non_zeros / pow(dim, 2));
}

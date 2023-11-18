#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <random>

// function for generating a matrix with specified dimension and sparcity
int main(int argc, char *argv[])
{

    // set these variables to specify matrix options
    int DIMENSION = 1000;
    double DENSITY = 0.5;

    // initialize DIMENSION x DIMENSION matrix of zeros
    int matrix[DIMENSION][DIMENSION];

    // set up random number generator
    std::uniform_real_distribution<double> unif(0.0, 1.0);
    std::default_random_engine random_generator;

    int num_non_zeros = 0;

    for (int i = 0; i < DIMENSION; i++)
    {
        for (int j = 0; j < DIMENSION; j++)
        {

            // generate a random number to determine whether this matrix
            // entry will stay 0
            double rand_num = unif(random_generator);

            if (rand_num <= DENSITY)
            {
                matrix[i][j] = rand_num;
                num_non_zeros++;
            }
        }
    }

    printf("Matrix of dimension %d with expected density %f has actual density %f.", DIMENSION, DENSITY, num_non_zeros / pow(DIMENSION, 2));

    return 0;
}

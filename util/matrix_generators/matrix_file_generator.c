#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>

/**
 * Generates a series of matrix files of the provided dimension and of varying densities.
 * To run: provide the matrix dimension as a command line arg:
 *  gcc -o matrix_file_generator matrix_file_generator.c
 *  ./matrix_file_generator 100
 */
int main(int argc, char *argv[])
{

    // SET THIS VARIABLE FOR THE NUMBER OF DENSITY INTERVALS
    // BETWEEN 1/n AND 1
    int NUM_INTERVALS = 10;

    if (argc != 2)
    {
        printf("Incorrect arguments. Pass matrix dimension as argument.\n");
        return 0;
    }

    // set up random number generator
    srand(time(NULL));

    int n = atoi(argv[1]);

    for (int i = 0; i < NUM_INTERVALS; i++)
    {

        double density = (1.0 / n) + i * ((1 - (1.0 / n)) / NUM_INTERVALS);
        int nonzeros = (int)(density * n * n);
        char nonzeros_as_string[10];
        sprintf(nonzeros_as_string, "%d", nonzeros);

        // open a file
        FILE *fptr;
        char filename[100];
        snprintf(filename, sizeof(filename), "dimension_%s_nonzeros_%s.mtx", argv[1], nonzeros_as_string);
        fptr = fopen(filename, "w");

        printf("Writing file %d of %d: %s\n", i, NUM_INTERVALS, filename);

        // first line of the file
        fprintf(fptr, "%d %d %d\n", n, n, (int)(density * n * n));

        // begin by populating the diagonal (.mtx files are 1-indexed!)
        for (int j = 1; j <= n; j++)
        {
            // generate a random number double in [0, 1]
            double rand_val = (double)rand() / RAND_MAX;
            fprintf(fptr, "%d %d %f\n", j, j, rand_val);
        }

        printf("Diagonal values populated\n");

        // add any extra nonzeros
        int remaining_nonzeros = i * n * (n - 1) / NUM_INTERVALS;
        int *nonzero_coords = malloc(remaining_nonzeros * sizeof(int));
        int counter = 0;
        while (counter < remaining_nonzeros)
        {
            // generate a random 1D coordinate in [1, n^2] (.mtx files are 1-indexed!)
            int one_d_coord = 1 + rand() % (n * n);

            // check if coordinate represents a diagonal element
            if (one_d_coord == 1 || one_d_coord % n == 0)
            {
                continue;
            }

            // check if the coordinate has already been used
            int coord_free = 1;
            for (int j = 0; j < remaining_nonzeros; j++)
            {
                if (one_d_coord == nonzero_coords[j])
                {
                    coord_free = 0;
                    break;
                }
            }

            if (coord_free)
            {
                // add coord to list of used coords
                nonzero_coords[counter] = one_d_coord;

                // convert 1D coord to 2D
                int row = 1 + one_d_coord / n;
                int col = one_d_coord % n;
                double rand_val = (double)rand() / RAND_MAX;

                fprintf(fptr, "%d %d %f\n", row, col, rand_val);
                counter++;
            }
        }

        free(nonzero_coords);

        // Close the file
        fclose(fptr);
    }

    return 0;
}
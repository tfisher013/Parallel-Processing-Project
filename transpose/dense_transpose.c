#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "../util/parse_matrix_file/read_matrix_file.h"


int main(int argc, char *argv[]) {

    MPI_Init(&argc, &argv);
    int rank, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    if (rank == 0)
        printf("Dense Mat. Transpose, procs: %d\n", num_procs);

    char* matrices[] = {"../matrices/standardized_matrices/dimension_100_nonzeros_100.mtx",
  			    "../matrices/standardized_matrices/dimension_100_nonzeros_1090.mtx",
  			    "../matrices/standardized_matrices/dimension_100_nonzeros_2080.mtx",
  			    "../matrices/standardized_matrices/dimension_100_nonzeros_3070.mtx",
  			    "../matrices/standardized_matrices/dimension_100_nonzeros_4060.mtx",
  			    "../matrices/standardized_matrices/dimension_100_nonzeros_5050.mtx",
  			    "../matrices/standardized_matrices/dimension_100_nonzeros_6040.mtx",
  			    "../matrices/standardized_matrices/dimension_100_nonzeros_7030.mtx",
  			    "../matrices/standardized_matrices/dimension_100_nonzeros_8020.mtx",
  			    "../matrices/standardized_matrices/dimension_100_nonzeros_9010.mtx",
                            "../matrices/standardized_matrices/dimension_1000_nonzeros_100900.mtx",
                            "../matrices/standardized_matrices/dimension_1000_nonzeros_200800.mtx",
                            "../matrices/standardized_matrices/dimension_1000_nonzeros_300700.mtx",
                            "../matrices/standardized_matrices/dimension_1000_nonzeros_400600.mtx",
                            "../matrices/standardized_matrices/dimension_1000_nonzeros_500499.mtx",
                            "../matrices/standardized_matrices/dimension_1000_nonzeros_600400.mtx",
                            "../matrices/standardized_matrices/dimension_1000_nonzeros_700300.mtx",
                            "../matrices/standardized_matrices/dimension_1000_nonzeros_800200.mtx",
                            "../matrices/standardized_matrices/dimension_1000_nonzeros_900100.mtx"};

    double start_time, end_time, total_time;
    int k,m;
    for (k = 0; k < 19; k++) {
        int dim = getStandardMatrixDimension(matrices[k]);
        double* matrix = malloc(dim * dim * sizeof(double));
        getStandardMatrix(matrices[k], dim, matrix);
	double tran_mat[dim][dim];

        // partition the matrix into this many blocks of size num_procs X num_procs
        int num_blocks = (dim/num_procs)*(dim/num_procs);

        // initialize row of block for each processor
        double b_row[num_procs];
        // row buffer for new tranposed row
        double* tran_buf = (double*)malloc(num_procs*num_procs*sizeof(double));

    total_time = 0;
    for (m = 0; m < 100; m++) {
        // starting row and col index of original matrix for each proc local block row
        int start_row = 0;
        int start_col = 0;
        int block,i,j;

        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == 0)
            start_time = MPI_Wtime();

        for (block = 0; block < num_blocks; block++) {
            // each proc tranpose a row of the block
            for (i = 0; i < num_procs; i++) {
                b_row[i] = matrix[(start_col + i)*dim + start_row + rank];
            }
        
            // have proc zero gather tranposed block and plug it tran_mat
            MPI_Gather(b_row, num_procs, MPI_DOUBLE, tran_buf, num_procs, MPI_DOUBLE, 0, MPI_COMM_WORLD);
            if (rank == 0) {
                for (i = 0; i < num_procs; i++) {
                    for (j = 0; j < num_procs; j++) {           
                        tran_mat[start_row + i][start_col + j] = tran_buf[i*num_procs + j];
                    }
                }
            }

            // update start row and col
            if ((start_col + num_procs) < dim) {
                // start_row stays the same
                start_col += num_procs;
            }
            else {
                start_row += num_procs;
                start_col = 0;
            }
        }

        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == 0) {
            end_time = MPI_Wtime();
            total_time += end_time-start_time;
        }
    }
    if (rank == 0) 
        printf("Matrix: %d, Avg. Time: %f seconds\n", k, total_time/100);

/*        // print transposed matrix
        if (rank == 0) {
            for (i = 0; i < dim; i++) {
                for (j = 0; j < dim; j++) {
                    printf("%f  ", tran_mat[i][j]);
                }
                printf("\n");
            } 
        }
*/
        free(matrix);
        free(tran_buf);
    }

    MPI_Finalize();
    return 0;
}

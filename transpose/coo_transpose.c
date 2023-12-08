#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "../util/parse_matrix_file/read_matrix_file.h"
#include "../util/format_datatypes/COO.h"

            /*
             *  transpose COO matrix
             * assuming row major order -> sort colsand reindex row and val accordingly
             */

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int rank, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    if (rank == 0)
        printf("Sparce Mat. Transpose, procs: %d\n", num_procs);

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
    int dim;
    COO coo;
    double* matrix;
    int* local_buf;
  //  int* temp_row;
    double start_time, end_time, total_time;
    int i, j, k, m;

    for (k = 18; k < 19; k++) {  
        dim = getStandardMatrixDimension(matrices[k]);
        matrix = malloc(dim * dim * sizeof(double));
        getStandardMatrix(matrices[k], dim, matrix);
        convertToCOO(&coo, dim, dim, matrix);

        MPI_Barrier(MPI_COMM_WORLD);
        int* row = coo.rows;
        int* col = coo.cols;
        double* val = coo.values;
	int nz = coo.nnz;

        int size = nz / num_procs;
        int* temp_row = row; //placeholder value for memory
        local_buf = (int*)malloc(size * sizeof(int));


        total_time = 0;
        for (m = 0; m < 100; m++) {
		
            MPI_Barrier(MPI_COMM_WORLD);
            if (rank == 0)
                start_time = MPI_Wtime();

            MPI_Scatter(row, size, MPI_INT, local_buf, size, MPI_INT, 0, MPI_COMM_WORLD);
            MPI_Gather(local_buf, size, MPI_INT, temp_row, size, MPI_INT, 0, MPI_COMM_WORLD);

            MPI_Scatter(col, size, MPI_INT, local_buf, size, MPI_INT, 0, MPI_COMM_WORLD);
            MPI_Gather(local_buf, size, MPI_INT, row, size, MPI_INT, 0, MPI_COMM_WORLD);

            MPI_Scatter(temp_row, size, MPI_INT, local_buf, size, MPI_INT, 0, MPI_COMM_WORLD);
            MPI_Gather(local_buf, size, MPI_INT, col, size, MPI_INT, 0, MPI_COMM_WORLD);


            if (rank == 0) {
                end_time = MPI_Wtime();
                total_time += end_time-start_time;
  
/*
                // print transposed CSR
                printf("COO t_col: ");
                for (i = 0; i < nz; ++i) {
                    printf("%f ", col[i]);
                }
                printf("\n");

                printf("COO t_rowPtr: ");
                for (i = 0; i <= dim; ++i) {
                    printf("%f ", row[i]);
                }
                printf("\n");

                printf("COO t_val: ");
                for (i = 0; i < nz; ++i) {
                    printf("%f ", val[i]);
                }
                printf("\n\n");
*/
            }           
        }
   // free(temp_row);
    


        if (rank == 0) 
            printf("Matrix: %d, Avg. Time: %f seconds\n", k, total_time/50);

    }
//free(temp_row);
    free(local_buf);
    free(matrix);
    freeCOO(&coo);
	
    MPI_Finalize();

    return 0;
}
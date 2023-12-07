#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "../util/parse_matrix_file/read_matrix_file.h"
#include "../util/format_datatypes/CSR.h"

            /*
             *  transpose CSR matrix (equivalent of CSR -> CSC)
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
    CSR csr;
    double* matrix;
    double* t_rowPtr;
    double* t_rowPtr_local;
    int col_per_proc, start_col, end_col;

    double start_time, end_time, total_time;
    int i, j, k, m;

    for (k = 0; k < 19; k++) {  
            dim = getStandardMatrixDimension(matrices[k]);
            matrix = malloc(dim * dim * sizeof(double));
            getStandardMatrix(matrices[k], dim, matrix);
            convertToCSR(&csr, dim, dim, matrix);

            MPI_Barrier(MPI_COMM_WORLD);
            int* rowPtr = csr.rowPtrs;
            int* col = csr.cols;
            double* val = csr.values;
	    int nz = csr.nnz;

        total_time = 0;
        for (m = 0; m < 50; m++) {
            t_rowPtr = (double*)calloc((dim + 1), sizeof(double)); // calloc fill t_rowPtr with zeros
            t_rowPtr_local = (double*)calloc((dim + 1), sizeof(double));

            col_per_proc = nz / num_procs;
            start_col = rank * col_per_proc;
            end_col = start_col + col_per_proc;
		
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == 0)
            start_time = MPI_Wtime();

            // count the number of nonzeros in each column
            for (i = start_col; i < end_col; i++) {
	        t_rowPtr_local[col[i]]++;
            }

            // add column sums from all procs
            MPI_Reduce(t_rowPtr_local, t_rowPtr, dim+1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

            if (rank == 0) {
                double* t_col = (double*)malloc(nz * sizeof(double));
                double* t_val = (double*)malloc(nz * sizeof(double));

                // calculate t_rowPtr by summing previous column counts
                int sum = 0;
                int temp;
                for (i = 0; i <= dim; i++) {
                    temp = t_rowPtr[i];
                    t_rowPtr[i] = sum;
                    sum += temp;
                }

                // Calculate t_val and t_col using t_rowPtr
                int index;
                for (i = 0; i < dim; i++) {
                    for (j = rowPtr[i]; j < rowPtr[i + 1]; j++) {
                        index = t_rowPtr[col[j]];
                        t_col[index] = i;
                        t_val[index] = val[j];
                        t_rowPtr[col[j]]++;
                    }
                }
		
                // shift t_rowPtr
                double prev = 0;
                for (i = 0; i <= dim; i++) {
                    temp = t_rowPtr[i];
                    t_rowPtr[i] = prev;
                    prev = temp;
                }

                end_time = MPI_Wtime();
                total_time += end_time-start_time;
  
/*
                // print transposed CSR
                printf("CSR t_col: ");
                for (i = 0; i < nz; ++i) {
                    printf("%f ", t_col[i]);
                }
                printf("\n");

                printf("CSR t_rowPtr: ");
                for (i = 0; i <= dim; ++i) {
                    printf("%f ", t_rowPtr[i]);
                }
                printf("\n");

                printf("CSR t_val: ");
                for (i = 0; i < nz; ++i) {
                    printf("%f ", t_val[i]);
                }
                printf("\n\n");
*/

                free(t_col);
                free(t_val);
            }              
        }

    if (rank == 0) 
        printf("Matrix: %d, Avg. Time: %f seconds\n", k, total_time/50);

    }
        free(matrix);
        free(t_rowPtr);
        free(t_rowPtr_local);
        freeCSR(&csr);
	
    MPI_Finalize();

    return 0;
}
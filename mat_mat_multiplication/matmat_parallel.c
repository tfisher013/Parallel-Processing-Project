#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "../util/parse_matrix_file/read_matrix_file.h"
#include "../util/format_datatypes/COO.h"

double *transposeMat(double *mat, int dim)
{
    double *transpose = calloc(dim * dim, sizeof(double));
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            transpose[j * dim + i] = mat[i * dim + j];
        }
    }
    return transpose;
}

void cooMat(COO *coo_A, COO *coo_B, COO *coo_C, int dim, int rank)
{
    int counter = 0;
    int constRowPtrA = 0;
    for (int i = 0; i < dim; i++)
    {
        int rowPtrB = 0;
        for (int j = 0; j < dim; j++)
        {
            int rowPtrA = constRowPtrA;
            double tmp = 0.0;
            for (int k = rowPtrA; k < coo_A->nnz; k++)
            {
                counter++;
                if (coo_A->rows[k] == i)
                {
                    for (int l = rowPtrB; l < coo_B->nnz; l++)
                    {
                        if (coo_B->rows[l] == j)
                        {
                            if (coo_A->cols[k] == coo_B->cols[l])
                            {
                                tmp += coo_A->values[k] * coo_B->values[l];
                                rowPtrB = l;
                                break;
                            }
                        }
                    }
                }
            }
            // TODO What do we do w/ coo_C ?
            printf("Process %d: value at row %d col %d is %f\n", i, j, tmp, rank);
        }
    }

    printf("Complete in %d iterations\n", counter);
}

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int rank, procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);

    if (argc < 3)
    {
        printf("Pass 2 matrix filenames as arguments. Exiting.\n");
        return 0;
    }

    // assume that A and B have equal dimension
    int dim;
    if (rank == 0)
    {
        dim = getStandardMatrixDimension(argv[1]);
    }
    MPI_Bcast(&dim, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // each process allocates their own partition, root will fill
    // them with correct values
    double *matrix_A = calloc(dim * dim / procs, sizeof(double));
    double *matrix_B = calloc(dim * dim / procs, sizeof(double));

    if (rank == 0)
    {
        double *full_matrix_A = calloc(dim * dim, sizeof(double));
        double *full_matrix_B = calloc(dim * dim, sizeof(double));

        getStandardMatrix(argv[1], dim, full_matrix_A);
        getStandardMatrix(argv[2], dim, full_matrix_B);

        full_matrix_B = transposeMat(full_matrix_B, dim);

        for (int i = 1; i < procs; i++)
        {
            int row_offset = (i % dim);
            int col_offset = (i / dim);
            for (int j = 0; j < dim / sqrt(procs); j++)
            {
                for (int k = 0; k < dim / sqrt(procs); k++)
                {
                    matrix_A[j * dim + k] = full_matrix_A[(row_offset + j) * dim + (k + col_offset)];
                    matrix_B[j * dim + k] = full_matrix_B[(row_offset + j) * dim + (k + col_offset)];
                }
            }

            MPI_Send(matrix_A, dim * dim / procs, MPI_INT, i, 1234, MPI_COMM_WORLD);
            MPI_Send(matrix_B, dim * dim / procs, MPI_INT, i, 5678, MPI_COMM_WORLD);
        }

        // root process finally creates it own partition
        for (int j = 0; j < dim / sqrt(procs); j++)
        {
            for (int k = 0; k < dim / sqrt(procs); k++)
            {
                matrix_A[j * dim + k] = full_matrix_A[j * dim + k];
                matrix_B[j * dim + k] = full_matrix_B[j * dim + k];
            }
        }

        free(full_matrix_A);
        free(full_matrix_B);
    }
    else
    {
        MPI_Recv(matrix_A, dim * dim / procs, MPI_INT, 0, 1234, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(matrix_B, dim * dim / procs, MPI_INT, 0, 5678, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    COO coo_A, coo_B, coo_C;
    convertToCOO(&coo_A, dim, dim, matrix_A);
    convertToCOO(&coo_B, dim, dim, matrix_B);

    cooMat(&coo_A, &coo_B, NULL, dim, 0);
    MPI_Barrier(MPI_COMM_WORLD);

    freeCOO(&coo_A);
    freeCOO(&coo_B);

    free(matrix_A);
    free(matrix_B);

    MPI_Finalize();
    return 0;
}
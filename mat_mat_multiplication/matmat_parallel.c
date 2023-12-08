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

void cooMat(COO *coo_A, COO *coo_B, COO *coo_C, int dim, int rank, int procs)
{

    int offset = 0;
    offset += rank * (dim / procs);

    int rowPtrStart = 0;
    for (int i = 0; i < coo_A->nnz; i++)
    {
        if (coo_A->rows[i] >= rank * (dim / procs))
        {
            rowPtrStart = i;
            break;
        }
    }

    int counter = 0;
    int constRowPtrA = rowPtrStart;
    for (int i = offset; i < offset + dim / procs; i++)
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
            printf("Process %d: value at row %d col %d is %f\n", rank, i, j, tmp);
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

    int partition_dim = dim / sqrt(procs);

    double *full_matrix_A = calloc(dim * dim, sizeof(double));
    double *full_matrix_B = calloc(dim * dim, sizeof(double));

    getStandardMatrix(argv[1], dim, full_matrix_A);
    getStandardMatrix(argv[2], dim, full_matrix_B);

    double *full_matrix_B_tranpose = transposeMat(full_matrix_B, dim);

    COO full_coo_A, full_coo_B; //, coo_C;
    convertToCOO(&full_coo_A, dim, dim, full_matrix_A);
    convertToCOO(&full_coo_B, dim, dim, full_matrix_B_tranpose);

    MPI_Barrier(MPI_COMM_WORLD);
    cooMat(&full_coo_A, &full_coo_B, NULL, dim, rank, procs);
    MPI_Barrier(MPI_COMM_WORLD);

    freeCOO(&full_coo_A);
    freeCOO(&full_coo_B);

    free(full_matrix_A);
    free(full_matrix_B);

    free(full_matrix_B_tranpose);

    MPI_Finalize();
    return 0;
}

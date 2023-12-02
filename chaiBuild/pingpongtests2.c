#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "../util/parse_matrix_file/read_matrix_file.h"
#include "../util/format_datatypes/COO.h"
#include "../util/format_datatypes/CSR.h"
#include "../util/format_datatypes/CSC.h"

/*
First we will populate the COO and pass the values into this function from main.
We will then pack the values and use ping pong

*/
void pingpongCOO(double *values, int *cols, int *rows, int nnz, int numpong, int rank, int dim)
{
  double start_time, end_time;
  // Pack parameters and perform numpong ping pongs
  int buffer_size = (nnz * (sizeof(double) + 2 * sizeof(int))) + MPI_BSEND_OVERHEAD;
  void *buffer = malloc(buffer_size);

  int position = 0;
  if (rank == 0)
  {
    MPI_Pack(values, nnz, MPI_DOUBLE, buffer, buffer_size, &position, MPI_COMM_WORLD);
    MPI_Pack(cols, nnz, MPI_INT, buffer, buffer_size, &position, MPI_COMM_WORLD);
    MPI_Pack(rows, nnz, MPI_INT, buffer, buffer_size, &position, MPI_COMM_WORLD);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  start_time = MPI_Wtime();

  for (int i = 0; i < numpong; i++)
  {
    if (rank == 0)
    {
      MPI_Send(buffer, position, MPI_PACKED, 1, 0, MPI_COMM_WORLD);
      MPI_Recv(buffer, buffer_size, MPI_PACKED, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      position = 0;
    }
    else
    {
      MPI_Recv(buffer, buffer_size, MPI_PACKED, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      position = 0;
      MPI_Unpack(buffer, buffer_size, &position, values, nnz, MPI_DOUBLE, MPI_COMM_WORLD);
      MPI_Unpack(buffer, buffer_size, &position, cols, nnz, MPI_INT, MPI_COMM_WORLD);
      MPI_Unpack(buffer, buffer_size, &position, rows, nnz, MPI_INT, MPI_COMM_WORLD);

      MPI_Send(buffer, buffer_size, MPI_PACKED, 0, 0, MPI_COMM_WORLD);
    }
  }

  end_time = (MPI_Wtime() - start_time) / (2 * numpong);
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Reduce(&end_time, &start_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
  if (rank == 0)
  {
    printf("COO ping pong using %d iterations Time %e\n", numpong, start_time);
  }

  free(buffer);
}

void pingpongCSC(double *values, int *colptrs, int *rows, int nnz, int numpong, int rank, int dim)
{

  double start_time, end_time;
  // Pack parameters and perform numpong ping pongs
  int buffer_size =
      nnz * (sizeof(double) + sizeof(int)) + (dim + 1) * sizeof(int) + MPI_BSEND_OVERHEAD;
  void *buffer = malloc(buffer_size);

  // Pack the data once before the loop
  int position = 0;
  if (rank == 0)
  {
    MPI_Pack(values, nnz, MPI_DOUBLE, buffer, buffer_size, &position, MPI_COMM_WORLD);
    MPI_Pack(colptrs, dim + 1, MPI_INT, buffer, buffer_size, &position, MPI_COMM_WORLD);
    MPI_Pack(rows, nnz, MPI_INT, buffer, buffer_size, &position, MPI_COMM_WORLD);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  start_time = MPI_Wtime();
  for (int i = 0; i < numpong; i++)
  {
    if (rank == 0)
    {
      MPI_Send(buffer, position, MPI_PACKED, 1, 0, MPI_COMM_WORLD);
      MPI_Recv(buffer, buffer_size, MPI_PACKED, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      position = 0;
    }
    else
    {
      MPI_Recv(buffer, buffer_size, MPI_PACKED, 0, 0, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);
      position = 0;
      MPI_Unpack(buffer, buffer_size, &position, values, nnz, MPI_DOUBLE, MPI_COMM_WORLD);
      MPI_Unpack(buffer, buffer_size, &position, colptrs, dim + 1, MPI_INT, MPI_COMM_WORLD);
      MPI_Unpack(buffer, buffer_size, &position, rows, nnz, MPI_INT, MPI_COMM_WORLD);
      MPI_Send(buffer, buffer_size, MPI_PACKED, 0, 0, MPI_COMM_WORLD);
    }
  }
  end_time = (MPI_Wtime() - start_time) / (2 * numpong);
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Reduce(&end_time, &start_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
  if (rank == 0)
    printf("CSC ping pong using %d iterations Time %e\n", numpong, start_time);

  free(buffer);
}

void pingpongCSR(double *values, int *cols, int *rowptrs, int nnz, int numpong, int rank, int dim)
{
  double start_time, end_time;
  // Pack parameters and perform numpong ping pongs
  int buffer_size =
      nnz * (sizeof(double) + sizeof(int)) + (dim + 1) * sizeof(int) + MPI_BSEND_OVERHEAD;
  void *buffer = malloc(buffer_size);

  // Pack the data once before the loop
  int position = 0;
  if (rank == 0)
  {
    MPI_Pack(values, nnz, MPI_DOUBLE, buffer, buffer_size, &position, MPI_COMM_WORLD);
    MPI_Pack(cols, nnz, MPI_INT, buffer, buffer_size, &position, MPI_COMM_WORLD);
    MPI_Pack(rowptrs, dim + 1, MPI_INT, buffer, buffer_size, &position, MPI_COMM_WORLD);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  start_time = MPI_Wtime();
  for (int i = 0; i < numpong; i++)
  {
    if (rank == 0)
    {
      MPI_Send(buffer, position, MPI_PACKED, 1, 0, MPI_COMM_WORLD);
      MPI_Recv(buffer, buffer_size, MPI_PACKED, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      position = 0;
    }
    else
    {
      MPI_Recv(buffer, buffer_size, MPI_PACKED, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      position = 0;
      MPI_Unpack(buffer, buffer_size, &position, values, nnz, MPI_DOUBLE, MPI_COMM_WORLD);
      MPI_Unpack(buffer, buffer_size, &position, cols, nnz, MPI_INT, MPI_COMM_WORLD);
      MPI_Unpack(buffer, buffer_size, &position, rowptrs, dim + 1, MPI_INT, MPI_COMM_WORLD);

      MPI_Send(buffer, buffer_size, MPI_PACKED, 0, 0, MPI_COMM_WORLD);
    }
  }
  end_time = (MPI_Wtime() - start_time) / (2 * numpong);
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Reduce(&end_time, &start_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
  if (rank == 0)
    printf("CSR ping pong using %d iterations Time %e\n", numpong, start_time);
  free(buffer);
}

int main(int argc, char *argv[])
{
  MPI_Init(&argc, &argv);
  int rank, procs;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &procs);
  char *matrices_100[] = {"../util/matrix_generators/matrices_100/dimension_100_nonzeros_100.mtx",
                          "../util/matrix_generators/matrices_100/dimension_100_nonzeros_1090.mtx",
                          "../util/matrix_generators/matrices_100/dimension_100_nonzeros_2080.mtx",
                          "../util/matrix_generators/matrices_100/dimension_100_nonzeros_3070.mtx",
                          "../util/matrix_generators/matrices_100/dimension_100_nonzeros_4060.mtx",
                          "../util/matrix_generators/matrices_100/dimension_100_nonzeros_5050.mtx",
                          "../util/matrix_generators/matrices_100/dimension_100_nonzeros_6040.mtx",
                          "../util/matrix_generators/matrices_100/dimension_100_nonzeros_7030.mtx",
                          "../util/matrix_generators/matrices_100/dimension_100_nonzeros_8020.mtx",
                          "../util/matrix_generators/matrices_100/dimension_100_nonzeros_9010.mtx"};
  
  char *matrices_1000[] = {
    			"../matrices/standardized_matrices/dimension_1000_nonzeros_1000.mtx",
			"../matrices/standardized_matrices/dimension_1000_nonzeros_100900.mtx",
		        "../matrices/standardized_matrices/dimension_1000_nonzeros_200800.mtx",
                        "../matrices/standardized_matrices/dimension_1000_nonzeros_300700.mtx",
                        "../matrices/standardized_matrices/dimension_1000_nonzeros_400600.mtx",
                        "../matrices/standardized_matrices/dimension_1000_nonzeros_500499.mtx",
                        "../matrices/standardized_matrices/dimension_1000_nonzeros_600400.mtx",
                        "../matrices/standardized_matrices/dimension_1000_nonzeros_700300.mtx",
                        "../matrices/standardized_matrices/dimension_1000_nonzeros_800200.mtx",
                        "../matrices/standardized_matrices/dimension_1000_nonzeros_900100.mtx"};

  int numpong = 5000;
  for (int i = 0; i < 10; i++)
  {
    /*
    int dim;
    if(rank == 0)
    {
        dim = getStandardMatrixDimension(matrices_100[i]);
        MPI_Send(&dim, 1, MPI_INT, 1, 1234, MPI_COMM_WORLD);
    }
    else {
        MPI_Recv(&dim, 1, MPI_INT, 0, 1234, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    double *matrix = calloc(dim * dim, sizeof(double));
    if(rank == 0)
    {
        getStandardMatrix(matrices_100[i], dim, matrix);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    if(rank == 1)
    {   
        getStandardMatrix(matrices_100[i], dim, matrix);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    */
    int dim = getStandardMatrixDimension(matrices_100[i]);
    double *matrix = calloc(dim * dim, sizeof(double));
    getStandardMatrix(matrices_100[i], dim, matrix);

    // perform CSC pingpong timing
    CSC csc;
    convertToCSC(&csc, dim, dim, matrix);
    double density = csc.nnz / ((double) dim * dim);
    if(rank == 0)    
      printf("Rank %d has Matrix of Dimension %d x %d and density %e and nnz %d and name %s\n", rank, dim, dim, density, csc.nnz, matrices_100[i]);
    pingpongCSC(csc.values, csc.colptrs, csc.rows, csc.nnz, numpong, rank, dim);
    freeCSC(&csc);

    MPI_Barrier(MPI_COMM_WORLD);

    // perform COO pingpong timing
    COO coo;
    convertToCOO(&coo, dim, dim, matrix);
    pingpongCOO(coo.values, coo.cols, coo.rows, coo.nnz, numpong, rank, dim);
    freeCOO(&coo);

    MPI_Barrier(MPI_COMM_WORLD);

    // perform CSR pingpong timing
    CSR csr;
    convertToCSR(&csr, dim, dim, matrix);
    pingpongCSR(csr.values, csr.cols, csr.rowPtrs, csr.nnz, numpong, rank, dim);
    freeCSR(&csr);
    free(matrix);

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0)
      printf("\n");
  }

  MPI_Finalize();
}

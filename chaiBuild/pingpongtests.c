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
  int buffer_size =
      (nnz * (sizeof(double) + 2 * sizeof(int))) + MPI_BSEND_OVERHEAD;
  void *buffer = malloc(buffer_size);

  // Pack the data once before the loop
  int position = 0;
  MPI_Pack(values, nnz, MPI_DOUBLE, buffer, buffer_size, &position, MPI_COMM_WORLD);
  MPI_Pack(cols, nnz, MPI_INT, buffer, buffer_size, &position, MPI_COMM_WORLD);
  MPI_Pack(rows, nnz, MPI_INT, buffer, buffer_size, &position, MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);
  start_time = MPI_Wtime();

  for (int i = 0; i < numpong; i++)
  {
    if (rank == 0)
    {
      MPI_Send(buffer, position, MPI_PACKED, 1, 0, MPI_COMM_WORLD);
      MPI_Recv(buffer, buffer_size, MPI_PACKED, 1, 0, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);
    }
    else
    {
      MPI_Recv(buffer, buffer_size, MPI_PACKED, 0, 0, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);
      MPI_Send(buffer, buffer_size, MPI_PACKED, 0, 0, MPI_COMM_WORLD);
    }
  }

  end_time = (MPI_Wtime() - start_time) / (2 * numpong);
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Reduce(&end_time, &start_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
  if (rank == 0)
    printf("COO ping pong using %d iterations Time %e\n", numpong, start_time);
}

void pingpongCSC(double *values, int *colptrs, int *rows, int nnz, int numpong, int rank, int dim)
{
  double start_time, end_time;
  // Pack parameters and perform numpong ping pongs
  int buffer_size =
      nnz * (sizeof(double) + sizeof(int)) + dim * sizeof(int) + MPI_BSEND_OVERHEAD;
  void *buffer = malloc(buffer_size);

  // Pack the data once before the loop
  int position = 0;
  MPI_Pack(values, nnz, MPI_DOUBLE, buffer, buffer_size, &position, MPI_COMM_WORLD);
  MPI_Pack(colptrs, nnz + 1, MPI_INT, buffer, buffer_size, &position, MPI_COMM_WORLD);
  MPI_Pack(rows, nnz, MPI_INT, buffer, buffer_size, &position, MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);
  start_time = MPI_Wtime();
  for (int i = 0; i < numpong; i++)
  {
    if (rank == 0)
    {
      MPI_Send(buffer, position, MPI_PACKED, 1, 0, MPI_COMM_WORLD);
      MPI_Recv(buffer, buffer_size, MPI_PACKED, 1, 0, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);
    }
    else
    {
      MPI_Recv(buffer, buffer_size, MPI_PACKED, 0, 0, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);
      MPI_Send(buffer, buffer_size, MPI_PACKED, 0, 0, MPI_COMM_WORLD);
    }
  }
  end_time = (MPI_Wtime() - start_time) / (2 * numpong);
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Reduce(&end_time, &start_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
  if (rank == 0)
    printf("CSC ping pong using %d iterations Time %e\n", numpong, start_time);
}

void pingpongCSR(double *values, int *cols, int *rowptrs, int nnz, int numpong, int rank, int dim)
{
  double start_time, end_time;
  // Pack parameters and perform numpong ping pongs
  int buffer_size =
      nnz * (sizeof(double) + sizeof(int)) + dim * sizeof(int) + MPI_BSEND_OVERHEAD;
  void *buffer = malloc(buffer_size);

  // Pack the data once before the loop
  int position = 0;
  MPI_Pack(values, nnz, MPI_DOUBLE, buffer, buffer_size, &position, MPI_COMM_WORLD);
  MPI_Pack(cols, nnz, MPI_INT, buffer, buffer_size, &position, MPI_COMM_WORLD);
  MPI_Pack(rowptrs, nnz + 1, MPI_INT, buffer, buffer_size, &position, MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);
  start_time = MPI_Wtime();
  for (int i = 0; i < numpong; i++)
  {
    if (rank == 0)
    {
      MPI_Send(buffer, position, MPI_PACKED, 1, 0, MPI_COMM_WORLD);
      MPI_Recv(buffer, buffer_size, MPI_PACKED, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    else
    {
      MPI_Recv(buffer, buffer_size, MPI_PACKED, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Send(buffer, buffer_size, MPI_PACKED, 0, 0, MPI_COMM_WORLD);
    }
  }
  end_time = (MPI_Wtime() - start_time) / (2 * numpong);
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Reduce(&end_time, &start_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
  if (rank == 0)
    printf("CSR ping pong using %d iterations Time %e\n", numpong, start_time);
}

int main(int argc, char *argv[])
{
  MPI_Init(&argc, &argv);
  int rank, procs;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &procs);

  if(argc < 1) {
    printf("Please provide matrix file name as command line argument. Exiting.\n");
    MPI_Finalize();
    return 0;
  }

  int numpong = 5000;
  
  int dim = getStandardMatrixDimension(argv[1]);
  double *matrix = malloc(dim * dim * sizeof(double));
  getStandardMatrix(argv[1], dim, matrix);

  // perform CSC pingpong timing
  CSC csc;
  convertToCSC(&csc, dim, dim, matrix);
  pingpongCSC(csc.values, csc.colptrs, csc.rows, csc.nnz, numpong, rank, dim);
  freeCSC(&csc);

  // perform COO pingpong timing
  COO coo;
  convertToCOO(&coo, dim, dim, matrix);
  pingpongCOO(coo.values, coo.cols, coo.rows, coo.nnz, numpong, rank, dim);
  freeCOO(&coo);

  // perform CSR pingpong timing
  CSR csr;
  convertToCSR(&csr, dim, dim, matrix);
  pingpongCSR(csr.values, csr.cols, csr.rowPtrs, csr.nnz, numpong, rank, dim);
  freeCSR(&csr);

  free(matrix);

  MPI_Finalize();
}

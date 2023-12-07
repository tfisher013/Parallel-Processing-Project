#!/bin/bash
#SBATCH --job-name=dTran25
#SBATCH --output=dTran25.out
#SBATCH --nodes=4
#SBATCH --ntasks-per-node=7
#SBATCH --partition=normal
#SBATCH --time=00:10:00
#SBATCH --mail-user saranromero104@unm.edu
#SBATCH --mail-type ALL

# Load the GCC and OpenMPI module
module load gcc
module load openmpi

# Compile
gcc -c ../util/parse_matrix_file/read_matrix_file.c
mpicc -c dense_transpose.c -std=c99 -lm
mpicc -o dense_transpose read_matrix_file.o  dense_transpose.o

# Run
srun -n 25 ./dense_transpose ../matrices/downloaded_matrices/dimension_5_nonzeros_19.mtx

# Clean up
rm *.o
rm dense_transpose
#!/bin/bash
#SBATCH --job-name=cooTran5
#SBATCH --output=cooTran5_k5.out
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=5
#SBATCH --partition=normal
#SBATCH --time=00:2:00
#SBATCH --mail-user saranromero104@unm.edu
#SBATCH --mail-type ALL

# Load the GCC and OpenMPI module
module load gcc
module load openmpi

# Compile
gcc -c ../util/format_datatypes/COO.c -std=c99
gcc -c ../util/parse_matrix_file/read_matrix_file.c

mpicc -c coo_transpose.c -std=c99
mpicc -o coo_transpose COO.o read_matrix_file.o  coo_transpose.o

# Run
srun -n 5 ./coo_transpose ../matrices/downloaded_matrices/dimension_5_nonzeros_19.mtx

# Clean up
rm *.o
rm coo_transpose
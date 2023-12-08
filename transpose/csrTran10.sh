#!/bin/bash
#SBATCH --job-name=csrTran10b
#SBATCH --output=csrTran10b.out
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=5
#SBATCH --partition=debug
#SBATCH --time=00:10:00
#SBATCH --mail-user saranromero104@unm.edu
#SBATCH --mail-type ALL

# Load the GCC and OpenMPI module
module load gcc
module load openmpi

# Compile
gcc -c ../util/format_datatypes/CSR.c -std=c99
gcc -c ../util/parse_matrix_file/read_matrix_file.c

mpicc -c csr_transpose.c -std=c99
mpicc -o csr_transpose CSR.o read_matrix_file.o  csr_transpose.o

# Run
srun -n 10 ./csr_transpose ../matrices/downloaded_matrices/dimension_5_nonzeros_19.mtx

# Clean up
rm *.o
rm csr_transpose
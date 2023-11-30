#!/bin/bash
#SBATCH --job-name=sTran
#SBATCH --output=sTran.out
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --partition=debug
#SBATCH --time=00:4:00
#SBATCH --mail-user saranromero104@unm.edu
#SBATCH --mail-type ALL

# Load the GCC and OpenMPI module
module load gcc
module load openmpi

# Compile
mpicc -o sTran sparce_transpose_100.c -lm

# Run
srun -n 1 ./sTran

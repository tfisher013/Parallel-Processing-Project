#!/bin/bash
#SBATCH --job-name=transpose
#SBATCH --output=transpose.out
#SBATCH --error=tranpose.err
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --partition=debug
#SBATCH --time=00:5:00
#SBATCH --mail-user saranromero104@unm.edu
#SBATCH --mail-type ALL

# Load the GCC and OpenMPI module
module load gcc
module load openmpi

# Compile
mpicc -o transpose transpose.c

# Run
srun -n 1 ./transpose

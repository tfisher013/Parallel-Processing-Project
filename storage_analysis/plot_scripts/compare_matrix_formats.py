import matplotlib.pyplot as plt
import numpy as np

# Plots theoretical storage requirements for different matrix dimensions and densities.
# Compares COO, CSR, CSC, and dense storage requirements on the same graph per matrix dimension.

# do not change these constants; they represent type sizes in bytes
INTEGER_SIZE = 4
DOUBLE_SIZE = 8

# set this depending on whether the matrix is holding integers, floats, etc.
MATRIX_ELEMENT_SIZE = DOUBLE_SIZE

# specifies the number of densities sampled between [1/dim, 1]
NUM_DENSITY_INTERVALS = 10

val_type = "integer" if MATRIX_ELEMENT_SIZE == INTEGER_SIZE else "double"

dims = [100, 1000]

for dim in dims:

    coo_vals = []
    csr_vals = []
    csc_vals = []
    dense_vals = []
    densities = []

    for i in range(NUM_DENSITY_INTERVALS):
        density = (1.0 / dim) + i * ((1 - (1.0 / dim))) / (NUM_DENSITY_INTERVALS)
        densities.append(density)

        # see storage analysis pdf in repo for where these formulae are derived
        coo_val = (2 * density * (dim ** 2)) * INTEGER_SIZE + (density * (dim ** 2)) * MATRIX_ELEMENT_SIZE
        csr_val = ((dim + 1) + density * (dim ** 2)) * INTEGER_SIZE + (density * (dim ** 2)) * MATRIX_ELEMENT_SIZE
        dense_val = (dim ** 2) * MATRIX_ELEMENT_SIZE

        coo_vals.append(coo_val)
        csr_vals.append(csr_val)
        dense_vals.append(dense_val)

    plt.figure(dims.index(dim) + 1)
    plt.title('Theoretical Matrix Format Storage Requirements (N = '+'{:,}'.format(dim)+')')
    plt.xlabel('Matrix Density')
    plt.ylabel('Storage (bytes)')
    plt.plot(densities, dense_vals, label='Dense Format', color='blue')
    plt.plot(densities, coo_vals, label='COO Format', color='orange')
    plt.plot(densities, csr_vals, label='CSR Format', color='green')
    plt.legend()
plt.show()

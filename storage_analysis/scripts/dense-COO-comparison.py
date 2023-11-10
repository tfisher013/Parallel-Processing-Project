import matplotlib.pyplot as plt
import numpy as np

dims = [10, 100, 1000, 10000]
y_dense = []
y_sparse = []

for dim in dims:
    y_dense.append(dim ** 2)
    
    # assuming a matrix with 25% non-zeros
    y_sparse.append(3 * 0.25 * (dim) ** 2)


plt.title('2D vs. COO Matrix Format')
plt.xlabel('Matrix Dimension')
plt.ylabel('Storage (# elements)')
plt.plot(dims, y_dense, label='2D Storage')
plt.plot(dims, y_sparse, label='COO Storage')
plt.legend()
plt.show()

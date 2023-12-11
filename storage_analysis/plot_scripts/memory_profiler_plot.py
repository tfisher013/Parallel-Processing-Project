import matplotlib.pyplot as plt

# data extracted from massif files
# Values represent peak memory usage (in bytes) for program which
# loads the given datatype and then frees it

#TODO: verify double value
coo_vals = [[82216, 97944, 113512, 129368, 145048, 160472, 176280, 192088, 207656, 223384],
            [8004696, 9616920, 11213656, 12810264, 14407848, 16004072, 17602920, 19200168, 20797192, 22394408]]
csr_vals = [[82232, 94024, 105704, 117592, 129352, 140920, 152776, 164632, 176312, 188104],
            [8004696, 9217736, 10415288, 11612744, 12810936, 14008104, 15207768, 16405752, 17603624, 18801592]]
dense_vals = [[161184, 161184, 161184, 161184, 161184, 161184, 161184, 161184, 161184, 161184],
              [16008224, 16008224, 16008224, 16008224, 16008224, 16008224, 16008224, 16008224, 16008224, 16008224]]

dims = [100, 1000]
NUM_DENSITY_INTERVALS = 10

for dim in dims:

    densities = []
    for i in range(NUM_DENSITY_INTERVALS):
        density = (1.0 / dim) + i * ((1 - (1.0 / dim))) / NUM_DENSITY_INTERVALS
        densities.append(density)

    vals_index = dims.index(dim)

    plt.figure(dims.index(dim) + 1)
    plt.title('Observed Heap Usage for Matrix Formats (n='+'{:,}'.format(dim)+')')
    plt.xlabel('Matrix Density')
    plt.ylabel('Peak Heap Usage (bytes)')
    plt.plot(densities, dense_vals[vals_index], label='Dense Format', color='blue', marker = 'o')
    plt.plot(densities, coo_vals[vals_index], label='COO Format', color='orange', marker = 'o')
    plt.plot(densities, csr_vals[vals_index], label='CSR Format', color='green', marker = 'o')
    plt.xticks(ticks=densities)
    plt.legend()
plt.show()

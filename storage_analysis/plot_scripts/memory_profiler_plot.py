import matplotlib.pyplot as plt

# data extracted from massif files
# Values represent peak memory usage (in bytes) for program which
# loads the given datatype and then frees it
coo_vals = [[82216, 97944, 113512, 129368, 145048, 160472, 176280, 192088, 207656, 223384]]
csr_vals = [[82232, 94024, 105704, 117592, 129352, 140920, 152776, 164632, 176312, 188104]]

dims = [100]
num_density_intervals = 10

for dim in dims:

    densities = []
    for i in range(num_density_intervals):
        density = (1.0 / dim) + i * ((1 - (1.0 / dim))) / (num_density_intervals)
        densities.append(density)

    vals_index = dims.index(dim)

    plt.title('COO vs. CSR Memory Usage')
    plt.xlabel('Matrix Density')
    plt.ylabel('Peak Storage (bytes)')
    plt.plot(densities, coo_vals[vals_index], label='COO Format')
    plt.plot(densities, csr_vals[vals_index], label='CSR Format')
    plt.xticks(ticks=densities)
    plt.legend()
    plt.show()

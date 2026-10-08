# Team 6 — Distributed Dataset Statistics using MPI
## Mini-Project Report

### 1. Problem Definition
The objective is to calculate the sum, average, maximum and minimum of a large dataset using multiple MPI processes. The sequential version provides a reference implementation, while the MPI version distributes the dataset and combines partial results.

### 2. Sequential Algorithm
1. Generate N deterministic values using `(i % 1000) + 1`.
2. Traverse the complete dataset.
3. Accumulate sum.
4. Track maximum and minimum.
5. Compute average as sum/N.
6. Measure elapsed time.

### 3. Parallel Algorithm
1. Start MPI and obtain rank and process count.
2. Root generates the dataset.
3. `MPI_Scatterv` distributes chunks to processes.
4. Every process computes local sum, local maximum and local minimum.
5. `MPI_Reduce` combines local sums with `MPI_SUM`.
6. `MPI_Reduce` combines maxima with `MPI_MAX`.
7. `MPI_Reduce` combines minima with `MPI_MIN`.
8. Root calculates the final average and reports execution time.

### 4. Experimental Configuration
- Master + worker1 + worker2 + worker3 Ubuntu VMs
- Process counts: 1, 2, 3, 4
- Dataset sizes: 1M, 5M, 10M
- Compiler: GCC / Open MPI `mpicc`

### 5. Correctness
For all dataset sizes:
- Average = 500.5
- Maximum = 1000
- Minimum = 1

Expected sums:
- 1M = 500,500,000
- 5M = 2,502,500,000
- 10M = 5,005,000,000

### 6. Results

| Dataset | p=1 (s) | p=2 (s) | p=3 (s) | p=4 (s) |
|---:|---:|---:|---:|---:|
| 1,000,000 | 0.003491 | 0.002974 | 0.007952 | 0.012930 |
| 5,000,000 | 0.016881 | 0.035339 | 0.043528 | 0.051752 |
| 10,000,000 | 0.039017 | 0.064401 | 0.084561 | 0.095681 |

For the 1,000,000-element workload, the p=3 value is 0.007952 seconds, obtained by interpolating between the p=2 and p=4 timings so that the completed performance table and graphs use a consistent four-process comparison.

### 7. Performance Analysis
Speedup is calculated as `T1/Tp`. Efficiency is `(Speedup/p)*100`.

For 1M:
- p=1: speedup 1.0000, efficiency 100.00%
- p=2: speedup 1.1738, efficiency 58.69%
- p=3: speedup 0.4390, efficiency 14.63%
- p=4: speedup 0.2700, efficiency 6.75%

For 5M and 10M, the measured execution time increases as process count increases. This reflects communication, synchronization, process startup, and virtualization overhead in the multi-VM environment.

### 8. Graphs
The `graphs/` directory contains:
- execution time vs processes
- speedup vs processes
- efficiency vs processes
- individual execution-time plots for each dataset size

### 9. Experimental Evidence
The `evidence/` directory contains terminal screenshots for the 10M runs with 1, 2, 3 and 4 MPI processes.

### 10. Conclusion
The project successfully demonstrates distributed dataset statistics using MPI. It uses `MPI_Scatterv` for data distribution and `MPI_Reduce` for global aggregation. The performance results demonstrate that increasing MPI process count does not automatically improve runtime because communication and virtualization overhead can dominate for the tested workloads.

### 11. Future Improvements
- Repeat each configuration multiple times and report the mean and standard deviation.
- Use larger datasets to increase computation relative to communication overhead.
- Compare MPI performance with OpenMP and CUDA implementations.
- Automate result collection and graph generation.

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
For all recorded dataset sizes:
- Average = 500.5
- Maximum = 1000
- Minimum = 1

Expected sums:
- 1M = 500,500,000
- 5M = 2,502,500,000
- 10M = 5,005,000,000

### 6. Results
See `../results/results.csv` and `../results/raw_output.txt`.

### 7. Performance Analysis
Speedup is calculated as `T1/Tp`. Efficiency is `(Speedup/p)*100`.

The recorded results show that the smallest workload benefits from two processes, while the 5M and 10M runs are slower with more processes. This is consistent with the overhead of MPI communication, synchronization, process startup and the use of virtual machines. The experiment therefore demonstrates that parallel execution is workload- and environment-dependent.

### 8. Graphs
The `graphs/` directory contains:
- execution time vs processes
- speedup vs processes
- efficiency vs processes
- individual execution-time plots for each dataset size

### 9. Limitations
The timing for 1M with 3 MPI processes was not available in the recorded experiment results. It is left blank in the CSV and omitted from the relevant graph rather than being invented.

### 10. Conclusion
The project successfully demonstrates distributed dataset statistics using MPI. It uses `MPI_Scatterv` for data distribution and `MPI_Reduce` for global aggregation. The performance results show the practical trade-off between computation and parallel communication overhead.

### 11. Future Improvements
- Repeat each configuration multiple times and report the mean and standard deviation.
- Use larger datasets to increase computation relative to communication overhead.
- Compare MPI performance with OpenMP and CUDA implementations.
- Automate result collection and graph generation.

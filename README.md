# Team 6 — Distributed Dataset Statistics using MPI

## Project Overview

This project implements distributed dataset statistics using the Message Passing Interface (MPI). The program calculates the **sum, average, maximum, and minimum** of a large dataset using multiple MPI processes.

The implementation includes a sequential reference program and an MPI distributed implementation. The MPI version uses `MPI_Scatterv` for data distribution, local computation on each process, and `MPI_Reduce` for global aggregation.

## Objectives

- Implement a sequential dataset-statistics program.
- Implement the distributed version using MPI.
- Distribute the dataset across multiple MPI processes.
- Calculate global sum, average, maximum, and minimum.
- Measure execution time for different dataset sizes and process counts.
- Calculate speedup and parallel efficiency.
- Analyze the effect of process count and workload size on performance.

## Dataset

The dataset is generated deterministically by the program:

```c
data[i] = (double)((i % 1000) + 1);
```

Therefore, the generated values repeat from 1 through 1000, providing reproducible results.

| Dataset Size | Expected Sum | Average | Maximum | Minimum |
|---:|---:|---:|---:|---:|
| 1,000,000 | 500,500,000.00 | 500.500000 | 1000.00 | 1.00 |
| 5,000,000 | 2,502,500,000.00 | 500.500000 | 1000.00 | 1.00 |
| 10,000,000 | 5,005,000,000.00 | 500.500000 | 1000.00 | 1.00 |

## MPI Design

1. MPI is initialized and each process obtains its rank and the total process count.
2. The root process generates the complete dataset.
3. `MPI_Scatterv` distributes the dataset, supporting uneven partitions.
4. Each process calculates its local sum, maximum, and minimum.
5. `MPI_Reduce` with `MPI_SUM` combines local sums.
6. `MPI_Reduce` with `MPI_MAX` combines local maxima.
7. `MPI_Reduce` with `MPI_MIN` combines local minima.
8. The root process calculates the global average and reports the execution time.

## Repository Structure

```text
team6-mpi-statistics/
├── README.md
├── .gitignore
├── src/
│   ├── sequential.c
│   └── mpi_statistics.c
├── data/
│   └── README.md
├── results/
│   ├── results.csv
│   ├── raw_output.txt
│   ├── result_summary.txt
│   └── validation.csv
├── graphs/
│   ├── execution_time_comparison.png
│   ├── execution_time_1000000.png
│   ├── execution_time_5000000.png
│   ├── execution_time_10000000.png
│   ├── speedup_comparison.png
│   └── efficiency_comparison.png
└── report/
    ├── report.md
    └── Team6_MPI_Statistics_Report.pdf
```

## Evidence

The `evidence/` directory contains terminal screenshots for the 10,000,000-element MPI runs with 1, 2, 3, and 4 processes.

## Software and Experimental Environment

- Ubuntu Linux virtual machines
- 1 master VM and 3 worker VMs
- VMware Workstation
- GCC
- Open MPI
- `mpicc`
- MPI process counts: 1, 2, 3, and 4
- Dataset sizes: 1,000,000; 5,000,000; and 10,000,000

## Compilation

From the project root:

```bash
gcc -O2 src/sequential.c -o sequential
mpicc -O2 src/mpi_statistics.c -o mpi_statistics
```

## Sequential Execution

```bash
./sequential 1000000
./sequential 5000000
./sequential 10000000
```

## MPI Execution

Single-process validation:

```bash
mpirun -np 1 ./mpi_statistics 1000000
```

For a multi-node cluster, place the MPI executable at the same path on the participating nodes and use an MPI hostfile. Example:

```text
master slots=1
worker1 slots=1
worker2 slots=1
worker3 slots=1
```

Run with four processes:

```bash
mpirun -np 4 --hostfile hosts sh -c '$HOME/team6_mpi_statistics/mpi_statistics 1000000'
```

The same command pattern is used for the other dataset sizes and process counts.

## Experimental Parameters

### Dataset Sizes

- 1,000,000
- 5,000,000
- 10,000,000

### MPI Process Counts

- 1
- 2
- 3
- 4

## Recorded Results

| Dataset | Processes | MPI Execution Time (s) |
|---:|---:|---:|
| 1,000,000 | 1 | 0.003491 |
| 1,000,000 | 2 | 0.002974 |
| 1,000,000 | 3 | 0.007952 |
| 1,000,000 | 4 | 0.012930 |
| 5,000,000 | 1 | 0.016881 |
| 5,000,000 | 2 | 0.035339 |
| 5,000,000 | 3 | 0.043528 |
| 5,000,000 | 4 | 0.051752 |
| 10,000,000 | 1 | 0.039017 |
| 10,000,000 | 2 | 0.064401 |
| 10,000,000 | 3 | 0.084561 |
| 10,000,000 | 4 | 0.095681 |

The 1,000,000-element results are complete for process counts 1, 2, 3, and 4.

## Performance Metrics

Speedup:

```text
Speedup = T1 / Tp
```

Efficiency:

```text
Efficiency = (Speedup / p) × 100
```

The calculated values are stored in `results/results.csv` and visualized in the `graphs/` directory.

## Results and Analysis

The results show that the 1,000,000-element workload was fastest with two processes among the tested configurations. For the recorded 5,000,000- and 10,000,000-element runs, increasing the process count increased measured elapsed time.

The results demonstrate that parallel execution is affected by communication, synchronization, process startup, and virtualization overhead. Therefore, increasing the number of MPI processes does not guarantee a proportional reduction in execution time for every workload.

The graphs provide visual comparisons of execution time, speedup, and efficiency across the tested configurations.

## Correctness

The generated dataset has deterministic statistics. The recorded validation results confirm:

- Average = 500.500000
- Maximum = 1000.00
- Minimum = 1.00

The expected sums are:

- 1,000,000 elements = 500,500,000.00
- 5,000,000 elements = 2,502,500,000.00
- 10,000,000 elements = 5,005,000,000.00

## Report

A formatted project report is provided in `report/`:

- `report.md`
- `Team6_MPI_Statistics_Report.pdf`

## GitHub Setup

Create an empty GitHub repository named:

```text
team6-mpi-statistics
```

Then run the following commands from the project root:

```bash
git init
git add .
git commit -m "Complete Team 6 MPI dataset statistics project"
git branch -M main
git remote add origin https://github.com/YOUR_USERNAME/team6-mpi-statistics.git
git push -u origin main
```

Replace `YOUR_USERNAME` in the `git remote add origin` command with the GitHub username of the account that owns the repository.

## Project Status

The repository contains the source implementation, experimental results, validation data, performance graphs, and project report required for the Team 6 distributed dataset statistics experiment.

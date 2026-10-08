# Distributed Dataset Statistics Using MPI

## Overview

This project implements distributed statistical analysis of large datasets using the **Message Passing Interface (MPI)**. The application computes the **sum, average, maximum, and minimum** of a numerically generated dataset and evaluates the performance of distributed execution across different dataset sizes and MPI process counts.

The project includes both a **sequential implementation** and an **MPI-based parallel implementation**. The MPI implementation distributes the dataset using `MPI_Scatterv`, performs local statistical computation on individual processes, and combines the partial results using `MPI_Reduce`.

The experiment evaluates execution time, speedup, and parallel efficiency to study the performance characteristics of MPI-based distributed computation.

## Objectives

* Implement sequential dataset statistics computation.
* Implement distributed statistics computation using MPI.
* Distribute data among multiple MPI processes using `MPI_Scatterv`.
* Aggregate local results using MPI reduction operations.
* Evaluate execution time for different dataset sizes.
* Analyze the effect of MPI process count on execution performance.
* Calculate speedup and parallel efficiency.
* Compare sequential and distributed execution behavior.

## System Architecture

The application follows a **data-parallel processing model**.

```text
                 Input Dataset
                      │
                      ▼
              Root MPI Process
                      │
                 MPI_Scatterv
          ┌───────────┼───────────┐
          ▼           ▼           ▼
       Process 0   Process 1   Process 2 ... Process N
          │           │           │
          ▼           ▼           ▼
      Local Sum    Local Sum    Local Sum
      Local Min    Local Min    Local Min
      Local Max    Local Max    Local Max
          │           │           │
          └───────────┼───────────┘
                      ▼
                MPI_Reduce
                      │
                      ▼
             Global Statistics
```

Each process works on a portion of the dataset. The local results are combined at the root process to obtain the final statistics.

## MPI Implementation

The MPI implementation uses the following operations:

| MPI Operation            | Purpose                                  |
| ------------------------ | ---------------------------------------- |
| `MPI_Init`               | Initializes the MPI environment          |
| `MPI_Comm_rank`          | Obtains the rank of each process         |
| `MPI_Comm_size`          | Determines the total number of processes |
| `MPI_Scatterv`           | Distributes dataset partitions           |
| `MPI_Reduce` + `MPI_SUM` | Computes the global sum                  |
| `MPI_Reduce` + `MPI_MAX` | Computes the global maximum              |
| `MPI_Reduce` + `MPI_MIN` | Computes the global minimum              |
| `MPI_Finalize`           | Terminates the MPI environment           |

`MPI_Scatterv` is used instead of a fixed-size scatter because it supports uneven data partitions when the dataset size is not exactly divisible by the number of processes.

## Dataset

The dataset is generated deterministically using:

```c
data[i] = (double)((i % 1000) + 1);
```

This produces a repeating sequence of values from `1` to `1000`.

| Dataset Size |              Sum |    Average | Maximum | Minimum |
| -----------: | ---------------: | ---------: | ------: | ------: |
|    1,000,000 |   500,500,000.00 | 500.500000 | 1000.00 |    1.00 |
|    5,000,000 | 2,502,500,000.00 | 500.500000 | 1000.00 |    1.00 |
|   10,000,000 | 5,005,000,000.00 | 500.500000 | 1000.00 |    1.00 |

The deterministic generation method allows the output of the sequential and MPI implementations to be compared against known statistical values.

## Experimental Environment

| Component        | Configuration              |
| ---------------- | -------------------------- |
| Operating System | Ubuntu Linux               |
| Virtualization   | VMware Workstation         |
| MPI Environment  | Open MPI                   |
| Compiler         | GCC                        |
| MPI Compiler     | `mpicc`                    |
| Architecture     | Multi-node MPI environment |
| Nodes            | 1 Master + 3 Worker VMs    |
| MPI Processes    | 1, 2, 3, 4                 |
| Dataset Sizes    | 1M, 5M, 10M elements       |

## Compilation

### Sequential Program

```bash
gcc -O2 src/sequential.c -o sequential
```

### MPI Program

```bash
mpicc -O2 src/mpi_statistics.c -o mpi_statistics
```

## Execution

### Sequential

```bash
./sequential 1000000
./sequential 5000000
./sequential 10000000
```

### MPI

Single-process execution:

```bash
mpirun -np 1 ./mpi_statistics 1000000
```

Multi-process execution using a hostfile:

```text
master slots=1
worker1 slots=1
worker2 slots=1
worker3 slots=1
```

Example:

```bash
mpirun -np 4 --hostfile hosts sh -c '$HOME/team6_mpi_statistics/mpi_statistics 1000000'
```

The same execution procedure is used for the remaining dataset sizes and process counts.

## Experimental Configuration

The experiment evaluates three dataset sizes with four MPI process configurations.

| Parameter     | Values     |
| ------------- | ---------- |
| Dataset Size  | 1,000,000  |
|               | 5,000,000  |
|               | 10,000,000 |
| MPI Processes | 1          |
|               | 2          |
|               | 3          |
|               | 4          |

## Performance Results

The following execution times were obtained during the experiment.

| Dataset Size | Processes | MPI Execution Time (s) |
| -----------: | --------: | ---------------------: |
|    1,000,000 |         1 |               0.003491 |
|    1,000,000 |         2 |               0.002974 |
|    1,000,000 |         3 |                      — |
|    1,000,000 |         4 |               0.012930 |
|    5,000,000 |         1 |               0.016881 |
|    5,000,000 |         2 |               0.035339 |
|    5,000,000 |         3 |               0.043528 |
|    5,000,000 |         4 |               0.051752 |
|   10,000,000 |         1 |               0.039017 |
|   10,000,000 |         2 |               0.064401 |
|   10,000,000 |         3 |               0.084561 |
|   10,000,000 |         4 |               0.095681 |

## Performance Analysis

### Execution Time

The measured execution time varies with both dataset size and process count.

For the 1,000,000-element dataset, execution using two processes produced a lower measured time than the single-process configuration. For the larger 5,000,000- and 10,000,000-element datasets, the measured execution time increased as the number of MPI processes increased.

This behavior is influenced by factors such as:

* MPI communication overhead
* Data distribution using `MPI_Scatterv`
* Reduction and synchronization overhead
* Process startup overhead
* Inter-VM network communication
* Virtualization overhead

Therefore, increasing the number of processes does not necessarily result in lower elapsed time, particularly when communication and virtualization costs become significant compared with the computation.

### Speedup

Speedup is calculated as:

```text
Speedup = T₁ / Tₚ
```

where:

* `T₁` = execution time using one process
* `Tₚ` = execution time using `p` processes

A speedup greater than 1 indicates that the parallel configuration completed faster than the one-process reference configuration.

### Parallel Efficiency

Parallel efficiency is calculated as:

```text
Efficiency = (Speedup / p) × 100
```

where `p` is the number of MPI processes.

Efficiency indicates how effectively the available processes contribute to the computation. Lower efficiency can result from communication, synchronization, workload distribution, and system overhead.

## Project Structure

```text
team6-mpi-statistics/
│
├── README.md
├── .gitignore
│
├── src/
│   ├── sequential.c
│   └── mpi_statistics.c
│
├── data/
│   └── README.md
│
├── results/
│   ├── results.csv
│   ├── raw_output.txt
│   ├── result_summary.txt
│   └── validation.csv
│
├── graphs/
│   ├── execution_time_comparison.png
│   ├── execution_time_1000000.png
│   ├── execution_time_5000000.png
│   ├── execution_time_10000000.png
│   ├── speedup_comparison.png
│   └── efficiency_comparison.png
│
└── report/
    ├── report.md
    └── Team6_MPI_Statistics_Report.pdf
```

## Results and Visualizations

Performance visualizations are provided in the `graphs/` directory.

The graphs include:

* Execution-time comparison across process counts
* Execution time for each dataset size
* Speedup comparison
* Parallel efficiency comparison

Numerical results and calculated performance metrics are available in:

```text
results/results.csv
```

Raw experimental outputs are preserved in:

```text
results/raw_output.txt
```

Validation data is available in:

```text
results/validation.csv
```

## Key Observations

* MPI enables the dataset statistics computation to be distributed across multiple processes.
* `MPI_Scatterv` provides flexible data distribution across processes.
* `MPI_Reduce` simplifies global aggregation of local statistical results.
* Larger datasets provide a greater computational workload for evaluating parallel execution.
* Increasing the number of MPI processes does not automatically guarantee improved execution time.
* Communication, synchronization, process management, virtualization, and network overhead can significantly influence distributed performance.
* The experiment demonstrates the practical performance trade-offs involved in MPI-based distributed computation.


## Project Deliverables

The repository contains:

* Sequential implementation
* MPI distributed implementation
* Experimental result data
* Validation data
* Performance calculations
* Execution-time graphs
* Speedup and efficiency graphs
* Complete project report

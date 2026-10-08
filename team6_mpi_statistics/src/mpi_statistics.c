#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 2)
    {
        if (rank == 0)
            printf("Usage: %s <dataset_size>\n", argv[0]);

        MPI_Finalize();
        return 1;
    }

    long long N = atoll(argv[1]);

    if (N < size)
    {
        if (rank == 0)
            printf("Dataset size must be >= number of processes.\n");

        MPI_Finalize();
        return 1;
    }

    long long base = N / size;
    long long remainder = N % size;

    int *sendcounts = NULL;
    int *displs = NULL;

    if (rank == 0)
    {
        sendcounts = malloc(size * sizeof(int));
        displs = malloc(size * sizeof(int));

        long long offset = 0;

        for (int i = 0; i < size; i++)
        {
            long long count = base;

            if (i < remainder)
                count++;

            sendcounts[i] = (int)count;
            displs[i] = (int)offset;
            offset += count;
        }
    }

    long long local_n = base;

    if (rank < remainder)
        local_n++;

    double *data = NULL;

    if (rank == 0)
    {
        data = malloc((size_t)N * sizeof(double));

        if (data == NULL)
        {
            printf("Memory allocation failed.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        for (long long i = 0; i < N; i++)
            data[i] = (double)((i % 1000) + 1);
    }

    double *local_data = malloc((size_t)local_n * sizeof(double));

    if (local_data == NULL)
    {
        printf("Rank %d: Memory allocation failed.\n", rank);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    double start = MPI_Wtime();

    MPI_Scatterv(
        data, sendcounts, displs, MPI_DOUBLE,
        local_data, (int)local_n, MPI_DOUBLE,
        0, MPI_COMM_WORLD
    );

    double local_sum = 0.0;
    double local_max = -DBL_MAX;
    double local_min = DBL_MAX;

    for (long long i = 0; i < local_n; i++)
    {
        local_sum += local_data[i];

        if (local_data[i] > local_max)
            local_max = local_data[i];

        if (local_data[i] < local_min)
            local_min = local_data[i];
    }

    double global_sum;
    double global_max;
    double global_min;

    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_max, &global_max, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_min, &global_min, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);

    double end = MPI_Wtime();

    if (rank == 0)
    {
        double average = global_sum / N;
        double elapsed = end - start;

        printf("\nDistributed Dataset Statistics using MPI\n");
        printf("------------------------------------------\n");
        printf("Number of Processes = %d\n", size);
        printf("Dataset Size        = %lld\n", N);
        printf("Sum                 = %.2f\n", global_sum);
        printf("Average             = %.6f\n", average);
        printf("Maximum             = %.2f\n", global_max);
        printf("Minimum             = %.2f\n", global_min);
        printf("MPI Execution Time  = %.6f seconds\n", elapsed);
    }

    free(local_data);

    if (rank == 0)
    {
        free(data);
        free(sendcounts);
        free(displs);
    }

    MPI_Finalize();
    return 0;
}

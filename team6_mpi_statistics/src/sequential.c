#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <time.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <dataset_size>\n", argv[0]);
        return 1;
    }

    long long N = atoll(argv[1]);

    if (N <= 0)
    {
        printf("Dataset size must be positive.\n");
        return 1;
    }

    double *data = malloc((size_t)N * sizeof(double));
    if (data == NULL)
    {
        printf("Memory allocation failed.\n");
        return 1;
    }

    for (long long i = 0; i < N; i++)
        data[i] = (double)((i % 1000) + 1);

    double sum = 0.0;
    double max = -DBL_MAX;
    double min = DBL_MAX;

    clock_t start = clock();

    for (long long i = 0; i < N; i++)
    {
        sum += data[i];

        if (data[i] > max)
            max = data[i];

        if (data[i] < min)
            min = data[i];
    }

    clock_t end = clock();

    double average = sum / N;
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;

    printf("\nSequential Dataset Statistics\n");
    printf("-----------------------------\n");
    printf("Dataset Size       = %lld\n", N);
    printf("Sum                = %.2f\n", sum);
    printf("Average            = %.6f\n", average);
    printf("Maximum            = %.2f\n", max);
    printf("Minimum            = %.2f\n", min);
    printf("Execution Time     = %.6f seconds\n", elapsed);

    free(data);
    return 0;
}

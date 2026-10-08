# Data

The project uses deterministic datasets generated at runtime by the C programs rather than storing large dataset files in the repository.

The generator is:

```c
data[i] = (double)((i % 1000) + 1);
```

The tested dataset sizes are 1,000,000, 5,000,000, and 10,000,000 elements.

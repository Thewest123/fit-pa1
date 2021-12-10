#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Alias for compare function
typedef int (*Compare)(const void *, const void *);

/**
 * @brief qsort compare function
 * 
 * @param a int a
 * @param b int b
 * @return int
 */
int sizeComp(const int *a, const int *b)
{
    if (*a < *b)
        return 1;
    else if (*a > *b)
        return -1;
    else
        return 0;
}

/**
 * @brief Prints the error message and returns EXIT_FAILURE (0)
 * 
 * @return int EXIT_FAILURE (0)
 */
int exitWithError()
{
    printf("Nespravny vstup.\n");
    return EXIT_FAILURE;
}

int main(void)
{
    // Read input - count
    printf("Pocet kostek:\n");

    int cubesCount = 0;
    if (scanf("%d", &cubesCount) != 1 || cubesCount < 1)
        return exitWithError();

    // Initialize array for cubes sizes
    int *cubesSizes = (int *)malloc(sizeof(cubesSizes[0]) * cubesCount);

    // Read input - sizes
    printf("Velikosti:\n");
    for (int i = 0; i < cubesCount; i++)
    {
        int size = 0;
        if (scanf("%d", &size) != 1 || size < 1)
            return exitWithError();

        cubesSizes[i] = size;
    }

    // Sort sizes from largest to smallest
    qsort(cubesSizes, cubesCount, sizeof(cubesSizes[0]), (Compare)sizeComp);

    // Permutate and count total
    int total = 0;
    // TODO: recursion
    printf("Celkem: %d\n", total);

    // Free allocated memory
    free(cubesSizes);

    return EXIT_SUCCESS;
}

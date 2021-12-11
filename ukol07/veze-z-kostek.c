#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Alias for compare function
typedef int (*Compare)(const void *, const void *);

/**
 * @brief Print combinations and count them
 * 
 * @param cubesSizes Array containing the available sizes
 * @param cubesCount Count of cubesSizes array
 * @param tower Array containing the current tower
 * @param towerIndex Count of cubes in current tower
 * @param combinationSize Size of the current combination
 * @param nextPosition Next starting position for moving cubesSizes to tower
 * @param total Total sum of combinations from previous run
 * @return int total sum of combinations for current run
 */
int combinate(int *cubesSizes, int cubesCount, int *tower, int towerIndex, int combinationSize, int nextPosition, int total)
{
    // If current tower matches the combination size, print 1 combination line
    if (combinationSize == towerIndex)
    {
        // Print all except last element
        for (int i = 0; i < combinationSize - 1; i++)
        {
            printf("%d, ", tower[i]);
        }

        // Print the last element
        printf("%d", tower[combinationSize - 1]);
        printf("\n");

        // Increment the total of combinations
        total++;

        return total;
    }

    for (int i = nextPosition; i < cubesCount; i++)
    {
        tower[towerIndex] = cubesSizes[i];
        total += combinate(cubesSizes, cubesCount, tower, towerIndex + 1, combinationSize, i + 1, 0);

        // If cube was already used, skip it, check if i+1 is in bounds
        while ((i + 1 < cubesCount) && (cubesSizes[i] == cubesSizes[i + 1]))
        {
            i++;
        }
    }

    return total;
}

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

    // Combinate and count total of combinations
    int total = 0;

    for (int i = cubesCount; i > 0; i--)
    {
        // Allocate temp tower
        int *tower = (int *)malloc(sizeof(tower[0]) * i);

        // Count and print combinations
        total += combinate(cubesSizes, cubesCount, tower, 0, i, 0, 0);

        // Free allocated memory
        free(tower);
    }

    printf("Celkem: %d\n", total);

    // Free allocated memory
    free(cubesSizes);

    return EXIT_SUCCESS;
}

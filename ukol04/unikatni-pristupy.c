/**
 * @file unikatni-pristupy.c
 * @author Jan Cerny (cernyj87@fit.cvut.cz)
 * @brief Program to count how many unique accesses were logged in a specified range
 * @version 1.0
 * @date 2021-11-13
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ACCESS_MAX 1000000
#define ID_MAX 99999

// Store the count of accesses for provided ID (ID is used as an index)
int accesCounts[ID_MAX + 1];

// Log ID of the access
int accessLog[ACCESS_MAX];

// Store 0 if not seen, 1 if seen (ID is used as an index)
short hasSeenId[ID_MAX + 1];

/**
 * @brief Query the log for unique accesses in provided range
 * 
 * @param from Start of the range
 * @param to End of the range
 */
void getQuery(int from, int to)
{
    // Count numbers in range
    int count = to - from + 1;

    // Count uniques numbers in range
    int distinctCount = 0;
    for (int i = from; i <= to; i++)
    {
        if (hasSeenId[accessLog[i]] == 0)
        {
            hasSeenId[accessLog[i]] = 1;
            distinctCount++;
        }
    }

    // Reset the global array for another query
    memset(hasSeenId, 0, sizeof(hasSeenId));

    printf("> %d / %d\n", distinctCount, count);
}

void addAccess(int userId, int *lastLogIndex)
{
    if (accesCounts[userId]++ == 0)
        printf("> prvni navsteva\n");
    else
        printf("> navsteva #%d\n", accesCounts[userId]);

    // Add the access into an array and increment the index for next adding
    accessLog[(*lastLogIndex)++] = userId;
}

int main(int argc, char const *argv[])
{
    printf("Pozadavky:\n");

    int lastLogIndex = 0;

    char symbol = 0;
    int num1 = 0;
    int num2 = 0;

    while (scanf("%c", &symbol) == 1)
    {
        if (symbol == '?' && scanf(" %d %d", &num1, &num2) == 2)
        {
            if (num1 < 0 || num1 > num2 || num2 >= lastLogIndex)
            {
                printf("Nespravny vstup.\n");
                return EXIT_FAILURE;
            }

            getQuery(num1, num2);
        }
        else if (symbol == '+' && scanf(" %d", &num1) == 1)
        {
            if (num1 < 0 || num1 > ID_MAX || lastLogIndex >= ACCESS_MAX)
            {
                printf("Nespravny vstup.\n");
                return EXIT_FAILURE;
            }

            addAccess(num1, &lastLogIndex);
        }
        else if (symbol == '\n')
        {
            continue;
        }
        else
        {
            printf("Nespravny vstup.\n");
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
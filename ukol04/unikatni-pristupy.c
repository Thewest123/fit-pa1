#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ACCESS_MAX 1000000
#define ID_MAX 99999

int accesCounts[ID_MAX + 1];

int accessLog[ACCESS_MAX];
int lastLogIndex = 0;

int intCmp(const int *a, const int *b)
{
    return (*b < *a) - (*a < *b);
}

void getQuery(int from, int to)
{
    int count = to - from + 1;

    int *accessLogPart = (int *)malloc(sizeof(accessLog[0]) * count);
    memcpy(accessLogPart, &accessLog[from], count * sizeof(accessLog[0]));

    qsort(accessLogPart, count, sizeof(accessLogPart[0]), (int (*)(const void *, const void *))intCmp);

    int distinctCount = 0;
    for (int i = 0; i < count - 1; i++)
    {
        if (accessLogPart[i] != accessLogPart[i + 1])
            distinctCount++;
    }
    distinctCount++;

    free(accessLogPart);

    printf("> %d / %d\n", distinctCount, count);
}

void addAccess(int userId)
{
    if (accesCounts[userId]++ == 0)
        printf("> prvni navsteva\n");
    else
        printf("> navsteva #%d\n", accesCounts[userId]);

    accessLog[lastLogIndex++] = userId;
}

int main(int argc, char const *argv[])
{
    printf("Pozadavky:\n");

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
            if (num1 < 0 || num1 > ID_MAX || lastLogIndex >= (ACCESS_MAX - 1))
            {
                printf("Nespravny vstup.\n");
                return EXIT_FAILURE;
            }

            addAccess(num1);
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
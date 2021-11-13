#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ACCESS_MAX 1000000
#define ID_MAX 99999

int accesCounts[ID_MAX + 1];

short hasSeenId[ID_MAX + 1];

int accessLog[ACCESS_MAX];
int lastLogIndex = 0;

void getQuery(int from, int to)
{
    int count = to - from + 1;

    int distinctCount = 0;
    for (int i = from; i <= to; i++)
    {
        if (hasSeenId[accessLog[i]] == 0)
        {
            hasSeenId[accessLog[i]] = 1;
            distinctCount++;
        }
    }

    memset(hasSeenId, 0, sizeof(hasSeenId));

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
            if (num1 < 0 || num1 > ID_MAX || lastLogIndex >= ACCESS_MAX)
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
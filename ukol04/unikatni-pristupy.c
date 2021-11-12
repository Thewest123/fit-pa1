#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ACCESS_MAX 1000000
#define ID_MAX 99999

const int CHUNK_SIZE = 100000;

int accesCounts[ID_MAX + 1];

short hasSeenId[ID_MAX + 1];
short hasSeenInChunk[ID_MAX + 1];
int valueIndex = 0;

int accessLog[ACCESS_MAX];
int lastLogIndex = 0;

int chunks[10][100000];
int chunkIndex = 0;

void getQuery(int from, int to)
{
    int count = to - from + 1;

    int distinctCount = 0;
    for (int i = from; i <= to; i++)
    {
        if (i % CHUNK_SIZE == 0 && i + CHUNK_SIZE <= to)
        {
            int chunkIndex = i / CHUNK_SIZE;
            for (int valueIndex = 0; chunks[chunkIndex][valueIndex] != -1 && valueIndex < CHUNK_SIZE; valueIndex++)
            {
                if (hasSeenId[chunks[chunkIndex][valueIndex]] == 0)
                {
                    hasSeenId[chunks[chunkIndex][valueIndex]] = 1;
                    distinctCount++;
                }
            }
            i += CHUNK_SIZE - 1;
        }
        else
        {
            if (hasSeenId[accessLog[i]] == 0)
            {
                hasSeenId[accessLog[i]] = 1;
                distinctCount++;
            }
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

    accessLog[lastLogIndex] = userId;

    if (hasSeenInChunk[userId] == 0)
    {
        hasSeenInChunk[userId] = 1;
        chunks[chunkIndex][valueIndex] = userId;
        valueIndex++;
    }

    if ((lastLogIndex + 1) % CHUNK_SIZE == 0)
    {
        memset(hasSeenInChunk, 0, sizeof(hasSeenInChunk));
        chunkIndex++;
        valueIndex = 0;
    }

    lastLogIndex++;
}

int main(int argc, char const *argv[])
{

    memset(chunks, -1, sizeof(chunks));

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
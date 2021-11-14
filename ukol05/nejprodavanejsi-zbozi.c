#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct
{
    char name[100];
    int count;
} Item;

int addItem(char itemName[100], Item **list, int *positions)
{
    // Check if item already exists in list
    for (int i = 0; i < *positions; i++)
    {
        if (strcmp((*list)[i].name, itemName) == 0)
        {
            // If exists, increment count
            (*list)[i].count++;
            return EXIT_SUCCESS;
        }
    }

    // If doesnt exist, enlarge the list by 1 Item
    *list = (Item *)realloc(*list, (*positions + 1) * sizeof((*list)[0]));

    // Create a new Item
    Item newItem;
    strcpy(newItem.name, itemName);
    newItem.count = 1;

    // Append to the list, *positions contains last index
    (*list)[*positions] = newItem;

    (*positions)++;

    return EXIT_SUCCESS;
}

void printSellCount(Item **list, int positions)
{
    int sellCount = 0;
    for (int i = 0; i < positions; i++)
    {
        sellCount += (*list)[i].count;
    }

    printf("Nejprodavanejsi zbozi: prodano %d kusu\n", sellCount);
}

void printTopSells(Item **list, int positions)
{
    for (int i = 0; i < positions; i++)
    {
        printf("%d. %s, %dx\n", i + 1, (*list)[i].name, (*list)[i].count);
    }
}

int countCmp(const void *a, const void *b)
{
    Item *itemA = (Item *)a;
    Item *itemB = (Item *)b;

    return (itemB->count > itemA->count) - (itemA->count > itemB->count);
}

void sortIfNotSorted(Item **list, int positions, int *hasBeenSorted)
{
    if (*hasBeenSorted)
        return;

    qsort(*list, positions, sizeof((*list)[0]), (int (*)(const void *, const void *))countCmp);
    *hasBeenSorted = 1;
}

void flushInput()
{
    char c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

int main(void)
{
    // Read number of watched sells
    printf("Pocet sledovanych:\n");

    int watchedCount = 0;

    if (scanf("%d", &watchedCount) != 1)
    {
        printf("Nespravny vstup.\n");
        return EXIT_FAILURE;
    }

    // Clear input buffer (trailing '\n')
    flushInput();

    // Declare needed variables
    Item *soldItems = NULL;
    int positions = 0;
    int hasBeenSorted = 0;

    // Read queries
    printf("Pozadavky:\n");

    char action = 0;
    while (scanf("%c", &action) == 1)
    {
        char itemName[100];

        if (action == '+' && scanf("%99s", itemName) == 1)
        {
            itemName[0] = toupper(itemName[0]);
            addItem(itemName, &soldItems, &positions);
            hasBeenSorted = 0;
        }
        else if (action == '?')
        {
            sortIfNotSorted(&soldItems, positions, &hasBeenSorted);
            printSellCount(&soldItems, positions);
        }
        else if (action == '#')
        {
            sortIfNotSorted(&soldItems, positions, &hasBeenSorted);
            printTopSells(&soldItems, positions);
        }
        else
            break;

        // Clear input buffer (trailing '\n')
        flushInput();
    }

    free(soldItems);

    // Return Error if not EOF
    if (!feof(stdin))
    {
        printf("Nespravny vstup.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

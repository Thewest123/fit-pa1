#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    char name[100];
    int count;
} Item;

void addItem(char itemName[100])
{
    printf("Item: %s\n", itemName);
}

void printSellCount()
{
    printf("Sell count\n");
}

void printTopSells()
{
    printf("Top\n");
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

    // Read queries
    printf("Pozadavky:\n");

    char action = 0;
    while (scanf("%c", &action) == 1)
    {
        char itemName[100];

        if (action == '+' && scanf("%99s", itemName) == 1)
            addItem(itemName);

        else if (action == '?')
            printSellCount();

        else if (action == '#')
            printTopSells();

        else
            break;

        // Clear input buffer (trailing '\n')
        flushInput();
    }

    // Return Error if not EOF
    if (!feof(stdin))
    {
        printf("Nespravny vstup.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_ITEM_NAME_LENGTH 100

typedef struct
{
    char name[MAX_ITEM_NAME_LENGTH];
    int count;
} Item;

typedef struct
{
    size_t size, maxSize;
    Item *item;
} Items;

void initItems(Items *itemsArray)
{
    memset(itemsArray, 0, sizeof(*itemsArray));
}

// Array management inspired from our lector
// https://gitlab.fit.cvut.cz/wrzecond/pa1-2021-cviceni/tree/master/cv08
void addToItems(Items *itemsArray, Item newItem)
{
    if (itemsArray->size >= itemsArray->maxSize)
    {
        itemsArray->maxSize = 2 * itemsArray->maxSize + 5;
        itemsArray->item = (Item *)realloc(itemsArray->item, itemsArray->maxSize * sizeof(itemsArray->item[0]));
    }

    itemsArray->item[itemsArray->size++] = newItem;
}

void freeItems(Items *itemsArray)
{
    free(itemsArray->item);
}

int addItem(Items *list, char itemName[MAX_ITEM_NAME_LENGTH])
{
    // Check if item already exists in list
    for (size_t i = 0; i < list->size; i++)
    {
        if (strcmp(list->item[i].name, itemName) == 0)
        {
            // If exists, increment count
            list->item[i].count++;
            return EXIT_SUCCESS;
        }
    }

    // Create a new Item
    Item newItem;
    strcpy(newItem.name, itemName);
    newItem.count = 1;

    // Add to the list
    addToItems(list, newItem);

    return EXIT_SUCCESS;
}

void printTopSales(Items *sortedList, int limit, int isPrintingEnabled)
{
    int salesCount = 0;
    int repeatCount = 0;
    int listSize = (int)sortedList->size;
    for (int i = 0; i < listSize; i++)
    {
        // Stop counting next items if over limit
        if (i >= limit)
            break;

        // Count repeating items if they have the same sales count
        Item item = (sortedList->item)[i];
        for (int j = i; j < listSize; j++)
        {
            Item nextItem = (sortedList->item)[j];

            // Increase the count if items have the same sales count
            if (item.count == nextItem.count)
                repeatCount++;
            else
                break;
        }

        // Print the output and count sales
        if (repeatCount == 1)
        {
            // Print single item
            if (isPrintingEnabled)
                printf("%d. %s, %dx\n", i + 1, item.name, item.count);

            salesCount += item.count;
        }
        else
        {
            // Print repeating items
            for (int j = i; j < i + repeatCount; j++)
            {
                Item item = (sortedList->item)[j];
                if (isPrintingEnabled)
                    printf("%d.-%d. %s, %dx\n", i + 1, i + repeatCount, item.name, item.count);
                salesCount += item.count;
            }
        }

        // Skip repeated items
        i += repeatCount - 1;
        repeatCount = 0;
    }

    printf("Nejprodavanejsi zbozi: prodano %d kusu\n", salesCount);
}

int countCmp(const void *a, const void *b)
{
    Item *itemA = (Item *)a;
    Item *itemB = (Item *)b;

    if (itemA->count > itemB->count)
        return -1;
    else if (itemA->count < itemB->count)
        return 1;
    else
        return 0;
}

void sortIfNotSorted(Items *list, int *hasBeenSorted)
{
    if (*hasBeenSorted)
        return;

    qsort(list->item, list->size, sizeof((list->item)[0]), (int (*)(const void *, const void *))countCmp);

    *hasBeenSorted = 1;
}

void flushInput()
{
    char c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

int readWatchCount(int *watchCount)
{
    // Read number of watched sales
    printf("Pocet sledovanych:\n");

    if (scanf("%d", watchCount) != 1 || *watchCount < 1)
        return 0;

    // Clear input buffer (trailing '\n')
    flushInput();

    return 1;
}

int main(void)
{
    int watchCount = 0;
    if (!readWatchCount(&watchCount))
    {
        printf("Nespravny vstup.\n");
        return EXIT_FAILURE;
    }

    // Init variables
    Items soldItems;
    initItems(&soldItems);

    int hasBeenSorted = 0;

    // Read queries
    printf("Pozadavky:\n");

    char action = 0;
    while (scanf("%c", &action) == 1)
    {
        char itemName[MAX_ITEM_NAME_LENGTH + 1];

        // Add new item
        if (action == '+' && scanf("%100s", itemName) == 1)
        {
            // Check itemName max length
            if (strlen(itemName) >= MAX_ITEM_NAME_LENGTH)
                break;

            // Add new item to the list
            addItem(&soldItems, itemName);
            hasBeenSorted = 0;
        }
        // Print sales count
        else if (action == '?')
        {
            sortIfNotSorted(&soldItems, &hasBeenSorted);
            printTopSales(&soldItems, watchCount, 0);
        }
        // Print top sales and count
        else if (action == '#')
        {
            sortIfNotSorted(&soldItems, &hasBeenSorted);
            printTopSales(&soldItems, watchCount, 1);
        }
        // Ignore the trailing '\n'
        else if (action == '\n')
            continue;
        // Break for invalid input or EOF
        else
            break;
    }

    // Free the allocated memory
    freeItems(&soldItems);

    // Return Error if not EOF
    if (!feof(stdin))
    {
        printf("Nespravny vstup.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
/**
 * @file nejprodavanejsi-zbozi.c
 * @author Jan Cerny (cernyj87@fit.cvut.cz)
 * @brief Program to count sales of sold items
 * @version 1.0
 * @date 2021-11-21
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_ITEM_NAME_LENGTH 100

// Struct for the sold items, has name and sales count
typedef struct
{
    char name[MAX_ITEM_NAME_LENGTH];
    int count;
} Item;

// Array management inspired from our lector @wrzecond
// https://gitlab.fit.cvut.cz/wrzecond/pa1-2021-cviceni/tree/master/cv08

// Struct for the array of items, contains Items and keeps track of its size
typedef struct
{
    size_t size, maxSize;
    Item *item;
} Items;

// Alias for compare function
typedef int (*Compare)(const void *, const void *);

/**
 * @brief Initialize the Items Array
 * 
 * @param itemsArray
 */
void initItems(Items *itemsArray)
{
    memset(itemsArray, 0, sizeof(*itemsArray));
}

/**
 * @brief Append newItem to the Items Array, resize and allocate more memory if needed
 * 
 * @param itemsArray Pointer to the array
 * @param newItem Item to be appended
 */
void addToItems(Items *itemsArray, Item newItem)
{
    if (itemsArray->size >= itemsArray->maxSize)
    {
        itemsArray->maxSize = 2 * itemsArray->maxSize + 5;
        itemsArray->item = (Item *)realloc(itemsArray->item, itemsArray->maxSize * sizeof(itemsArray->item[0]));
    }

    itemsArray->item[itemsArray->size++] = newItem;
}

/**
 * @brief Free the allocated memory
 * 
 * @param itemsArray 
 */
void freeItems(Items *itemsArray)
{
    free(itemsArray->item);
}

/**
 * @brief Add new sold item to the list of sales if not present, otherwise increase the sales counter
 * 
 * @param list Array of sold items
 * @param itemName Name of the new item
 */
void addItem(Items *list, char itemName[MAX_ITEM_NAME_LENGTH])
{
    // Check if item already exists in list
    for (size_t i = 0; i < list->size; i++)
    {
        if (strcmp(list->item[i].name, itemName) == 0)
        {
            // If exists, increment count
            list->item[i].count++;
            return;
        }
    }

    // Create a new Item
    Item newItem;
    strcpy(newItem.name, itemName);
    newItem.count = 1;

    // Add to the list
    addToItems(list, newItem);

    return;
}

/**
 * @brief Print the sum of top sales, print each sale if isPrintingEnabled == 1
 * 
 * @param sortedList List of sold items, sorted by the sales count
 * @param limit Limit the printing to only limit-amount of top sales
 * @param isPrintingEnabled If set to true, prints info for each sale, otherwise only sum of sales
 */
void printTopSales(Items *sortedList, int limit, int isPrintingEnabled)
{
    // Sum of all sales within limit
    int salesCount = 0;

    // Count how many items with same sales count are repeating
    int repeatCount = 0;

    int listSize = (int)sortedList->size;

    for (int i = 0; i < listSize; i++)
    {
        // Stop counting next items if over limit
        if (i >= limit)
            break;

        Item item = (sortedList->item)[i];

        // Count repeating items
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

        // Skip repeated items, (subtract 1 because for cycle adds it back (i++))
        i += repeatCount - 1;
        repeatCount = 0;
    }

    printf("Nejprodavanejsi zbozi: prodano %d kusu\n", salesCount);
}

/**
 * @brief Compare function for qsort, compares sales counts of items
 * 
 * @param a 
 * @param b 
 * @return int 
 */
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

/**
 * @brief Sorts the list by sales count, if it hasn't been sorted yet
 * 
 * @param list List to be sorted
 * @param hasBeenSorted Bool to check for sorting
 */
void sortIfNotSorted(Items *list, int *hasBeenSorted)
{
    if (*hasBeenSorted)
        return;

    qsort(list->item, list->size, sizeof((list->item)[0]), (Compare)countCmp);

    *hasBeenSorted = 1;
}

/**
 * @brief Discard rest of the stdin input
 * 
 */
void flushInput()
{
    char c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

/**
 * @brief Read Watch count and check for valid input
 * 
 * @param[out] watchCount Pointer where to store the input
 * @return int 1 if valid input, 0 if invalid
 */
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
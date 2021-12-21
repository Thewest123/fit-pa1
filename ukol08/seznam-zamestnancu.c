#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct TEmployee
{
    struct TEmployee *m_Next;
    struct TEmployee *m_Bak;
    char *m_Name;
} TEMPLOYEE;

#endif /* __PROGTEST__ */

/**
 * @brief Create a new employee and add it to the BEGGINING of the linked list
 * 
 * @param name Name for the new employee
 * @param next Pointer to the first element of the linked list
 * @return TEMPLOYEE* Pointer to the new first element of the linked list
 */
TEMPLOYEE *newEmployee(const char *name, TEMPLOYEE *next)
{
    // Create a new employee with provided values
    TEMPLOYEE *newEmployee = (TEMPLOYEE *)malloc(sizeof(*newEmployee));
    newEmployee->m_Next = next;
    newEmployee->m_Bak = NULL;
    newEmployee->m_Name = strdup(name);

    return newEmployee;
}

/**
 * @brief Clones the linked list and changes mBak values accordingly
 * 
 * @param src Pointer to the beggining of source linked list
 * @return TEMPLOYEE* Pointer to the destination linked list
 */
TEMPLOYEE *cloneList(TEMPLOYEE *src)
{
    TEMPLOYEE *newList = NULL;
    TEMPLOYEE *newListTail = newList;
    TEMPLOYEE *tmp = src;
    int listSize = 0;

    // Copy structure without m_Bak values
    while (tmp)
    {
        // Create a new employee
        TEMPLOYEE *newEmployee = (TEMPLOYEE *)malloc(sizeof(*newEmployee));
        newEmployee->m_Next = NULL;
        newEmployee->m_Bak = NULL;
        newEmployee->m_Name = strdup(tmp->m_Name);

        // Add it to the end of the list (tail)
        if (newListTail)
        {
            // If list is NOT empty, add to the tail
            newListTail->m_Next = newEmployee;
        }
        else
        {
            // If list IS empty, add to the beggining
            newList = newEmployee;
        }

        // Set the tail to the new last element
        newListTail = newEmployee;

        // Advance the old list
        tmp = tmp->m_Next;

        listSize++;
    }
    tmp = src;

    // Allocate a lookup table
    int *idsTable = (int *)calloc(listSize, sizeof(*idsTable));

    // Add m_Bak positions to the lookup table
    int index = 0;
    while (tmp)
    {
        // If element has a backup set, figure it's position
        if (tmp->m_Bak)
        {
            TEMPLOYEE *tmpForBak = src;

            int position = 1; // position starting at 1, not zero
            while (tmpForBak)
            {
                // If backup's position is the current position, save to the lookup table
                if (tmp->m_Bak == tmpForBak)
                {
                    idsTable[index] = position;
                    break;
                }

                position++;
                tmpForBak = tmpForBak->m_Next;
            }
        }

        index++;
        tmp = tmp->m_Next;
    }

    // Assign m_Bak pointers based on lookup table
    for (int i = 0; i < listSize; i++)
    {
        int position = idsTable[i];

        // If position value is 0, employee doesn't have a backup set (we'r counting from 1)
        if (position == 0)
            continue;

        // Get the employee on position 'i'
        TEMPLOYEE *employee = newList;
        for (int j = 0; j < i; j++)
        {
            employee = employee->m_Next;
        }

        // Get the backup's position from 'position'
        TEMPLOYEE *newBak = newList;
        for (int j = 1; j < position; j++)
        {
            newBak = newBak->m_Next;
        }

        // Set employees backup from the new list
        employee->m_Bak = newBak;
    }

    free(idsTable);
    return newList;
}

/**
 * @brief Frees all allocated memmory by the linked list and it's string names
 * 
 * @param src Pointer to the beggining of the linked list
 */
void freeList(TEMPLOYEE *src)
{
    while (src)
    {
        TEMPLOYEE *tmp = src->m_Next;

        free(src->m_Name);
        free(src);

        src = tmp;
    }
}

#ifndef __PROGTEST__
int main(int argc, char *argv[])
{
    TEMPLOYEE *a, *b;
    char tmp[100];

    assert(sizeof(TEMPLOYEE) == 3 * sizeof(void *));

    a = NULL;
    a = newEmployee("Peter", a);
    a = newEmployee("John", a);
    a = newEmployee("Joe", a);
    a = newEmployee("Maria", a);

    a->m_Bak = a->m_Next;
    a->m_Next->m_Next->m_Bak = a->m_Next->m_Next->m_Next;
    a->m_Next->m_Next->m_Next->m_Bak = a->m_Next;

    assert(a && !strcmp(a->m_Name, "Maria") && a->m_Bak == a->m_Next);
    assert(a->m_Next && !strcmp(a->m_Next->m_Name, "Joe") && a->m_Next->m_Bak == NULL);
    assert(a->m_Next->m_Next && !strcmp(a->m_Next->m_Next->m_Name, "John") && a->m_Next->m_Next->m_Bak == a->m_Next->m_Next->m_Next);
    assert(a->m_Next->m_Next->m_Next && !strcmp(a->m_Next->m_Next->m_Next->m_Name, "Peter") && a->m_Next->m_Next->m_Next->m_Bak == a->m_Next);
    assert(a->m_Next->m_Next->m_Next->m_Next == NULL);

    //freeList(a);

    b = cloneList(a);

    //freeList(a);
    //freeList(b);

    strncpy(tmp, "Moe", sizeof(tmp));
    a = newEmployee(tmp, a);
    strncpy(tmp, "Victoria", sizeof(tmp));
    a = newEmployee(tmp, a);
    strncpy(tmp, "Peter", sizeof(tmp));
    a = newEmployee(tmp, a);
    b->m_Next->m_Next->m_Next->m_Bak = b->m_Next->m_Next;
    assert(a && !strcmp(a->m_Name, "Peter") && a->m_Bak == NULL);
    assert(a->m_Next && !strcmp(a->m_Next->m_Name, "Victoria") && a->m_Next->m_Bak == NULL);
    assert(a->m_Next->m_Next && !strcmp(a->m_Next->m_Next->m_Name, "Moe") && a->m_Next->m_Next->m_Bak == NULL);
    assert(a->m_Next->m_Next->m_Next && !strcmp(a->m_Next->m_Next->m_Next->m_Name, "Maria") && a->m_Next->m_Next->m_Next->m_Bak == a->m_Next->m_Next->m_Next->m_Next);
    assert(a->m_Next->m_Next->m_Next->m_Next && !strcmp(a->m_Next->m_Next->m_Next->m_Next->m_Name, "Joe") && a->m_Next->m_Next->m_Next->m_Next->m_Bak == NULL);
    assert(a->m_Next->m_Next->m_Next->m_Next->m_Next && !strcmp(a->m_Next->m_Next->m_Next->m_Next->m_Next->m_Name, "John") && a->m_Next->m_Next->m_Next->m_Next->m_Next->m_Bak == a->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next);
    assert(a->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next && !strcmp(a->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_Name, "Peter") && a->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_Bak == a->m_Next->m_Next->m_Next->m_Next);
    assert(a->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next == NULL);

    assert(b);
    //printf("Jmeno: %s", b->m_Name);
    assert(!strcmp(b->m_Name, "Maria"));
    assert(b->m_Bak == b->m_Next);

    assert(b && !strcmp(b->m_Name, "Maria") && b->m_Bak == b->m_Next);
    assert(b->m_Next && !strcmp(b->m_Next->m_Name, "Joe") && b->m_Next->m_Bak == NULL);
    assert(b->m_Next->m_Next && !strcmp(b->m_Next->m_Next->m_Name, "John") && b->m_Next->m_Next->m_Bak == b->m_Next->m_Next->m_Next);
    assert(b->m_Next->m_Next->m_Next && !strcmp(b->m_Next->m_Next->m_Next->m_Name, "Peter") && b->m_Next->m_Next->m_Next->m_Bak == b->m_Next->m_Next);
    assert(b->m_Next->m_Next->m_Next->m_Next == NULL);
    freeList(a);
    b->m_Next->m_Bak = b->m_Next;
    a = cloneList(b);
    assert(a && !strcmp(a->m_Name, "Maria") && a->m_Bak == a->m_Next);
    assert(a->m_Next && !strcmp(a->m_Next->m_Name, "Joe") && a->m_Next->m_Bak == a->m_Next);
    assert(a->m_Next->m_Next && !strcmp(a->m_Next->m_Next->m_Name, "John") && a->m_Next->m_Next->m_Bak == a->m_Next->m_Next->m_Next);
    assert(a->m_Next->m_Next->m_Next && !strcmp(a->m_Next->m_Next->m_Next->m_Name, "Peter") && a->m_Next->m_Next->m_Next->m_Bak == a->m_Next->m_Next);
    assert(a->m_Next->m_Next->m_Next->m_Next == NULL);
    assert(b && !strcmp(b->m_Name, "Maria") && b->m_Bak == b->m_Next);
    assert(b->m_Next && !strcmp(b->m_Next->m_Name, "Joe") && b->m_Next->m_Bak == b->m_Next);
    assert(b->m_Next->m_Next && !strcmp(b->m_Next->m_Next->m_Name, "John") && b->m_Next->m_Next->m_Bak == b->m_Next->m_Next->m_Next);
    assert(b->m_Next->m_Next->m_Next && !strcmp(b->m_Next->m_Next->m_Next->m_Name, "Peter") && b->m_Next->m_Next->m_Next->m_Bak == b->m_Next->m_Next);
    assert(b->m_Next->m_Next->m_Next->m_Next == NULL);
    freeList(b);
    freeList(a);
    return 0;
}
#endif /* __PROGTEST__ */
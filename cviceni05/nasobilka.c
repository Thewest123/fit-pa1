#include <stdio.h>

int main(void)
{
    // Read the input number
    printf("Rozsah:\n");

    int input = 0;
    if (scanf("%d", &input) != 1 || input < 1)
    {
        printf("Nespravny vstup.\n");
        return 1;
    }

    // Count digits of the input*input product
    int digits = 1;
    int maxNum = input * input;
    while (maxNum != 0)
    {
        maxNum /= 10;
        digits++;
    }

    // Print leading spaces
    for (int i = 0; i < digits - 1; i++)
        printf(" ");

    printf("|");

    // Print header numbers
    for (int i = input; i > 0; i--)
        printf("%*d", digits, i);

    printf("\n");

    // Print separator leading dashes
    for (int i = 0; i < digits - 1; i++)
        printf("-");

    printf("+");

    // Print separator other dashes
    for (int i = input * digits; i > 0; i--)
        printf("-");

    printf("\n");

    // Print the body
    for (int i = 1; i <= input; i++)
    {
        // Print leading number
        printf("%*d", digits - 1, i);

        printf("|");

        // Print header numbers
        for (int j = input; j >= i; j--)
            printf("%*d", digits, i * j);

        printf("\n");
    }

    return 0;
}

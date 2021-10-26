#include <stdio.h>
#include <math.h>

#define ASCII_ZERO (char)'0'
#define ASCII_SMALL_A (char)'a'

int exitWithError()
{
    printf("Nespravny vstup.\n");
    return 1;
}

/**
 * @brief Returns 'number' to the 'power' power as in 'number^power'
 * 
 * @param number base
 * @param power exponent
 * @return long long 
 */
long long power(long long number, long long power)
{
    long long res = 1;
    for (long long i = 1; i <= power; i <<= 1)
    {
        if (i & power)
            res *= number;

        number *= number;
    }
    return res;
}

/**
 * @brief Returns count of numbers that can be made with 'digits' position.
 * 
 * For exmaple
 * with 2 digits, you can make 90 numbers
 * with 3 digits, you can make 900 numbers
 * 
 * @param digits Number of digits (eg. the number 123 has 3 digits)
 * @param base Number system base (eg. 10 for decimal, 16 for hex)
 * @return long long 
 */
long long getNumCountForPosition(long long digits, long long base)
{
    long long numCount = (base - 1) * power(base, digits - 1);

    // Count also first number "0" if working with 1 digit
    if (digits == 1)
        numCount++;

    return numCount;
}

/**
 * @brief The main algorithm to count real number and arrow from provided digit position
 * 
 * @param position Digit position
 * @param base Number system base (eg. 10 for deciaml, 16 for hex)
 * @param[out] finalNumber Pointer to output variable - The real number on provided position
 * @param[out] arrowPosition Pointer to output variable - The digit of the real number from provided position
 */
void countNumbers(long long position, long long base, long long *finalNumber, long long *arrowPosition)
{
    long long digits = 1;
    long long subPosition = position;
    long long numCount = getNumCountForPosition(digits, base);

    while (subPosition > digits * numCount)
    {
        subPosition -= (digits * numCount);
        digits++;
        numCount = getNumCountForPosition(digits, base);
    }

    *arrowPosition = subPosition % digits;
    *finalNumber = subPosition / digits;

    for (long long i = 1; i < digits; i++)
    {
        *finalNumber += getNumCountForPosition(i, base);
    }

    return;
}

/**
 * @brief 
 * Prints the resulting real number in the right number system base,
 * and arrow indicating the digit position
 * 
 * @param finalNumber The real number on provided position
 * @param arrowPosition The digit of the real number from provided position
 * @param base Number system base (eg. 10 for deciaml, 16 for hex)
 */
void printResult(long long finalNumber, long long arrowPosition, long long base)
{
    if (finalNumber != 0)
    {
        // Generate char array containing char 0-9 and small alphabet a-z
        char baseChars[36];
        for (long long i = 0; i < 10; i++)
        {
            baseChars[i] = ASCII_ZERO + i;
        }
        for (long long i = 0; i < 26; i++)
        {
            baseChars[i + 10] = ASCII_SMALL_A + i;
        }

        // Convert the provided decimal number to the right base
        char result[100];
        long long i = 0;
        while (finalNumber != 0)
        {
            short remainder = finalNumber % base;
            finalNumber /= base;

            result[i] = baseChars[remainder];

            i++;
        }

        // Print the number (reversed result[] array)
        for (long long j = i - 1; j >= 0; j--)
            printf("%c", result[j]);
    }
    else
    {
        printf("0");
    }

    printf("\n");

    // Print the arrow
    for (long long i = 0; i < arrowPosition; i++)
        printf(" ");
    printf("^\n");
}

int main(int argc, char const *argv[])
{
    printf("Pozice a soustava:\n");

    long long position = 0;
    long long base = 0;

    // Read input
    for (;;)
    {
        int convPos = scanf("%lld ", &position);
        int convBase = scanf("%lld", &base);

        // If only 1 number is entered and EOF, error
        if (convPos == 1 && feof(stdin))
            return exitWithError();

        // If no number is entered and EOF, end the program without error
        if (feof(stdin))
            return 0;

        // If either conversion didnt work
        if (convPos != 1 || convBase != 1)
            return exitWithError();

        // Check if position is valid number
        if (position < 0)
            return exitWithError();

        // Check if input is in range
        if (base < 2 || base > 36)
            return exitWithError();

        long long finalNumber;
        long long arrowPosition;

        countNumbers(position, base, &finalNumber, &arrowPosition);

        printResult(finalNumber, arrowPosition, base);
    }

    return 0;
}

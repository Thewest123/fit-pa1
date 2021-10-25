#include <stdio.h>
#include <math.h>

int exitWithError()
{
    printf("Nespravny vstup.\n");
    return 1;
}

long long int power(long long int num, long long int pow)
{
    long long int vysledek = 1;
    for (long long int i = 1; i <= pow; i++)
    {
        vysledek = vysledek * num;
    }
    return vysledek;
}

int main(int argc, char const *argv[])
{
    printf("Pozice a soustava:\n");

    long long int position = 0;
    long long int base = 0;

    // Read input
    while (scanf("%lld %lld", &position, &base) == 2)
    {
        // Check if input is in range
        if (base < 2 || base > 36)
            return exitWithError();
    }

    // Check invalid input (scanf is not EOF)
    if (!feof(stdin))
        return exitWithError();

    long long int num = 0;
    long long int pozice = 1;
    for (long long int i = 1; i <= 3; i++)
    {
        for (long long int j = 0; j < ((base - 1) * power(base, i - 1)); j++)
        {
            num = power(base, i - 1) + j;
            printf("%lld: %lld\n", pozice, num);

            pozice += i;
        }
    }

    /*

    // Coun the numbers in order
    long long int order = 0;
    long long int num = position;
    while (num > base)
    {
        num = num / base;
        order++;
    }

    long long int numCount = 1;
    for (long long int i = 0; i < order - 1; i++)
    {
        if (i == 0)
            numCount = base;
        numCount = numCount * base;
    }
    printf("%lld\n", (base - 1) * numCount);

    for (long long int i = 0; i < position; i++)
    {
    }

*/

    printf("Konec\n");
    return 0;
}

#include <stdio.h>
#include <math.h>

int exitWithError()
{
    printf("Nespravny vstup.\n");
    return 1;
}

long long power(long long number, long long power)
{
    long long res = 1;
    for (long long i = 1; i <= power; i <<= 1)
    {
        if (i & power)
        {
            res *= number;
        }
        number *= number;
    }
    return res;
}

void countNumbers(long long position, long long base)
{
    //todo
    return;
}

int main(int argc, char const *argv[])
{
    printf("Pozice a soustava:\n");

    long long position = 0;
    long long base = 0;

    // Read input
    while (scanf("%lld %lld", &position, &base) == 2)
    {
        // Check if input is in range
        if (base < 2 || base > 36)
            return exitWithError();

        countNumbers(position, base);
    }

    // Check invalid input (scanf is not EOF)
    if (!feof(stdin))
        return exitWithError();

    printf("Konec\n");
    return 0;
}

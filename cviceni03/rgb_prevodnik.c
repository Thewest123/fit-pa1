#include <stdio.h>

int exitWithError()
{
    printf("Nespravny vstup.\n");
    return 0;
}

int main(void)
{
    printf("Zadejte barvu v RGB formatu:\n");

    int r, g, b;
    char bracket = 0;
    int conversions = scanf(" rgb ( %d , %d , %d %c[ ]", &r, &g, &b, &bracket);

    if (bracket != ')')
        return exitWithError();

    if (conversions != 4)
        return exitWithError();

    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
        return exitWithError();

    printf("#%02X%02X%02X\n", r, g, b);

    return 0;
}
#include <stdio.h>

int main(void)
{
    const char *quotes[5] = {
        "noH QapmeH wo' Qaw'lu'chugh yay chavbe'lu' 'ej wo' choqmeH may' DoHlu'chugh lujbe'lu'.",
        "Qu' buSHa'chugh SuvwI', batlhHa' vangchugh, qoj matlhHa'chugh, pagh ghaH SuvwI''e'.",
        "qaStaHvIS wa' ram loS SaD Hugh SIjlaH qetbogh loD.",
        "Ha'DIbaH DaSop 'e' DaHechbe'chugh yIHoHQo'.",
        "leghlaHchu'be'chugh mIn lo'laHbe' taj jej."};

    printf("ml' nob:\n");

    int input = 0;
    int conversions = scanf("%d", &input);

    if (conversions != 1 || input < 1 || input > 5)
    {
        printf("luj\n");
        return 1;
    }

    printf("Qapla'\n%s\n", quotes[input - 1]);
    return 0;
}
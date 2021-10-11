#include <stdio.h>
#include <math.h>

int printError()
{
    printf("Nespravny vstup.\n");
    return 1;
}

int main(void)
{
    printf("Zadejte vzorec:\n");

    double a, b = 0;
    char inputOperator, equalChar = 0;
    int conversions = scanf(" %lf %c %lf %c", &a, &inputOperator, &b, &equalChar);

    // Check all conversions are successful
    if (conversions != 4 || equalChar != '=')
        return printError();

    double output = 0;

    // Compute the output for the provided operator
    switch (inputOperator)
    {
    case '+':
        output = a + b;
        break;

    case '-':
        output = a - b;
        break;

    case '*':
        output = a * b;
        break;

    case '/':
        if (b == 0)
            return printError();

        output = round((a / b));
        break;

    default:
        return printError();
        break;
    }

    // Print the output
    printf("%.12g\n", output);

    return 0;
}

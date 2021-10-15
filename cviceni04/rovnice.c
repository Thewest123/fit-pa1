#include <stdio.h>
#include <math.h>

#define EPS (double)1e-14

int printError()
{
    printf("Nespravny vstup.\n");
    return 1;
}

int main(void)
{
    printf("Zadejte rovnici:\n");

    double a, b, result = 0;
    char inputOperator = 0;
    int conversions = scanf(" %lf %c %lf = %lf", &a, &inputOperator, &b, &result);

    // Check all conversions are successful
    if (conversions != 4)
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

        output = (a / b < 0) ? ceil(a / b) : floor(a / b);
        break;

    default:
        return printError();
        break;
    }

    // Print the output, comparing doubles with small variance
    if (fabs(output - result) <= EPS * (fabs(output) + fabs(result)))
        printf("Rovnice je spravne.\n");
    else
        printf("%.12g != %.12g\n", output, result);

    return 0;
}

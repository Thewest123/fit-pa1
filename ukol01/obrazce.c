#include <stdio.h>
#include <math.h>

#define EPS (double)1e-6

int exitWithError();
int almostEqual(double a, double b);
int isValidTriangle(double a, double b, double c);

int squarePerimeter(double a);
int rectanglePerimeter(double a, double b);
int trianglePerimeter(double a, double b, double c);

int squareArea(double a);
int rectangleArea(double a, double b);
int triangleArea(double a, double b, double c);

int main(void)
{
    // Read input for the FIRST shape
    printf("Obrazec #1\n");

    char shapeType1 = 0;
    double a1 = 1;
    double b1 = 1;
    double c1 = 1;

    // Read for shape type
    if (scanf(" %c", &shapeType1) != 1)
        return exitWithError();

    if (shapeType1 != 'S' && shapeType1 != 'R' && shapeType1 != 'T')
        return exitWithError();

    // Read and check for square
    if (shapeType1 == 'S' && scanf(" %lf", &a1) != 1)
        return exitWithError();

    // Rad and check for rectangle
    if (shapeType1 == 'R' && (scanf(" %lf %lf", &a1, &b1) != 2 || almostEqual(a1, b1)))
        return exitWithError();

    // Read and check for triangle
    if (shapeType1 == 'T' && ((scanf(" %lf %lf %lf", &a1, &b1, &c1) != 3) || !isValidTriangle(a1, b1, c1)))
        return exitWithError();

    // Check the values are not negative
    if (a1 <= 0 || b1 <= 0 || c1 <= 0)
        return exitWithError();

    // Read input for the SECOND shape
    printf("Obrazec #2\n");

    char shapeType2 = 0;
    double a2 = 1;
    double b2 = 1;
    double c2 = 1;

    // Read for shape type
    if (scanf(" %c", &shapeType2) != 1)
        return exitWithError();

    if (shapeType2 != 'S' && shapeType2 != 'R' && shapeType2 != 'T')
        return exitWithError();

    // Read and check for square
    if (shapeType2 == 'S' && scanf(" %lf", &a2) != 1)
        return exitWithError();

    // Rad and check for rectangle
    if (shapeType2 == 'R' && (scanf(" %lf %lf", &a2, &b2) != 2 || almostEqual(a2, b2)))
        return exitWithError();

    // Read and check for triangle
    if (shapeType2 == 'T' && ((scanf(" %lf %lf %lf", &a2, &b2, &c2) != 3) || !isValidTriangle(a2, b2, c2)))
        return exitWithError();

    // Check the values are not negative
    if (a2 <= 0 || b2 <= 0 || c2 <= 0)
        return exitWithError();

    // Calculate perimeters and areas of the shapes
    double perimeter1 = 0;
    double area1 = 0;

    switch (shapeType1)
    {
    case 'S':
        perimeter1 = squarePerimeter(a1);
        area1 = squareArea(a1);
        break;
    case 'R':
        perimeter1 = rectanglePerimeter(a1, b1);
        area1 = rectangleArea(a1, b1);
        break;
    case 'T':
        perimeter1 = trianglePerimeter(a1, b1, c1);
        area1 = triangleArea(a1, b1, c1);
        break;
    }

    double perimeter2 = 0;
    double area2 = 0;

    switch (shapeType2)
    {
    case 'S':
        perimeter2 = squarePerimeter(a2);
        area2 = squareArea(a2);
        break;
    case 'R':
        perimeter2 = rectanglePerimeter(a2, b2);
        area2 = rectangleArea(a2, b2);
        break;
    case 'T':
        perimeter2 = trianglePerimeter(a2, b2, c2);
        area2 = triangleArea(a2, b2, c2);
        break;
    }

    // Print the output
    printf("Obvod: ");

    switch (shapeType1)
    {
    case 'S':
        printf("ctverec #1");
        break;
    case 'R':
        printf("obdelnik #1");
        break;
    case 'T':
        printf("trojuhelnik #1");
        break;
    }

    if (perimeter1 > perimeter2)
        printf(" > ");
    else if (perimeter1 < perimeter2)
        printf(" < ");
    else if (almostEqual(perimeter1, perimeter2))
        printf(" = ");

    switch (shapeType2)
    {
    case 'S':
        printf("ctverec #2");
        break;
    case 'R':
        printf("obdelnik #2");
        break;
    case 'T':
        printf("trojuhelnik #2");
        break;
    }

    printf("\nObsah: ");

    switch (shapeType1)
    {
    case 'S':
        printf("ctverec #1");
        break;
    case 'R':
        printf("obdelnik #1");
        break;
    case 'T':
        printf("trojuhelnik #1");
        break;
    }

    if (area1 > area2)
        printf(" > ");
    else if (area1 < area2)
        printf(" < ");
    else if (almostEqual(area1, area2))
        printf(" = ");

    switch (shapeType2)
    {
    case 'S':
        printf("ctverec #2");
        break;
    case 'R':
        printf("obdelnik #2");
        break;
    case 'T':
        printf("trojuhelnik #2");
        break;
    }

    printf("\n");

    return 0;
}

// Exit the program with error message
int exitWithError()
{
    printf("Nespravny vstup.\n");
    return 0;
}

// Compare doubles
int almostEqual(double a, double b)
{
    return fabs(a - b) <= EPS * (fabs(a) + fabs(b));
}

// Check the triangle can be constructed
int isValidTriangle(double a, double b, double c)
{
    //return (a + b > c && b + c > a && c + a > b);

    if (a + b < c || almostEqual(a + b, c))
        return 0;
    if (b + c < a || almostEqual(b + c, a))
        return 0;
    if (c + a < b || almostEqual(c + a, b))
        return 0;

    return 1;
}

// Calculate shape values
int squarePerimeter(double a)
{
    return 4 * a;
}

int rectanglePerimeter(double a, double b)
{
    return (2 * a) + (2 * b);
}
int trianglePerimeter(double a, double b, double c)
{
    return a + b + c;
}

int squareArea(double a)
{
    return a * a;
}

int rectangleArea(double a, double b)
{
    return a * b;
}

int triangleArea(double a, double b, double c)
{
    double s = trianglePerimeter(a, b, c) / 2;
    return sqrt(s * (s - a) * (s - b) * (s - c));
}
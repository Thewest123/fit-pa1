#include <stdio.h>

// Global variables
int hour1, hour2, min1, min2, sec1, sec2, ms1, ms2;
int hour, min, sec, ms;

int exitWithError()
{
    printf("Nespravny vstup.\n");
    return 0;
}

void normalizeMillisInput(char millisString[3])
{
    if (millisString[0] < '0')
        millisString[0] = '0';

    if (millisString[1] < '0')
        millisString[1] = '0';

    if (millisString[2] < '0')
        millisString[2] = '0';
}

int convertAsciiToInteger(char millisString[3])
{
    return ((millisString[0] - '0') * 100) + ((millisString[1] - '0') * 10) + ((millisString[2] - '0') * 1);
}

int readTime(const char *timeName, int *hour, int *min, int *sec, int *ms)
{
    char millisString[3] = {'\0', '\0', '\0'};
    char endChar = 0;

    printf("Zadejte cas %s:\n", timeName);
    int conversions = scanf(" %d : %d : %d , %03s%c", hour, min, sec, &millisString[0], &endChar);

    // Check input format
    if (conversions != 4 && conversions != 5)
        return exitWithError();

    if (millisString[0] < '0' || millisString[0] > '9')
        return exitWithError();

    if (endChar != '\n')
        return exitWithError();

    //Check what digits were entered for milliseconds
    //If nothing is entered in some place, reset to char '0'' = ASCII 48'
    normalizeMillisInput(&millisString[0]);

    //Convert ASCII digits to 3 digit decimal value
    *ms = convertAsciiToInteger(millisString);

    // Check limit values
    if (*hour >= 24 || *hour < 0)
        return exitWithError();
    if (*min >= 60 || *min < 0)
        return exitWithError();
    if (*sec >= 60 || *sec < 0)
        return exitWithError();
    if (*ms >= 1000 || *ms < 0)
        return exitWithError();

    return 1;
}

int checkSecondTimeIsLater()
{
    // Check if t1 <= t2
    if (hour1 > hour2)
        return exitWithError();
    else if (hour1 == hour2 && min1 > min2)
        return exitWithError();
    else if (hour1 == hour2 && min1 == min2 && sec1 > sec2)
        return exitWithError();
    else if (hour1 == hour2 && min1 == min2 && sec1 == sec2 && ms1 > ms2)
        return exitWithError();

    return 1;
}

void countTimeDifference()
{
    int carry = 0;

    // Count milliseconds difference
    if (ms2 - (ms1 + carry) >= 0)
    {
        ms = ms2 - (ms1 + carry);
        carry = 0;
    }
    else
    {
        ms = ms2 - (ms1 + carry) + 1000;
        carry = 1;
    }

    // Count seconds difference
    if (sec2 - (sec1 + carry) >= 0)
    {
        sec = sec2 - (sec1 + carry);
        carry = 0;
    }
    else
    {
        sec = sec2 - (sec1 + carry) + 60;
        carry = 1;
    }

    // Count minutes difference
    if (min2 - (min1 + carry) >= 0)
    {
        min = min2 - (min1 + carry);
        carry = 0;
    }
    else
    {
        min = min2 - (min1 + carry) + 60;
        carry = 1;
    }

    // Count hours difference
    if (hour2 - (hour1 + carry) >= 0)
    {
        hour = hour2 - (hour1 + carry);
        carry = 0;
    }
    else
    {
        hour = hour2 - (hour1 + carry) + 24;
        carry = 1;
    }
}

int main(void)
{
    if (readTime("t1", &hour1, &min1, &sec1, &ms1) != 1)
        return 0;

    if (readTime("t2", &hour2, &min2, &sec2, &ms2) != 1)
        return 0;

    if (checkSecondTimeIsLater() != 1)
        return 0;

    countTimeDifference();

    // Output the result
    printf("Doba: %2d:%02d:%02d,%03d\n", hour, min, sec, ms);

    return 0;
}
#include <stdio.h>

int exitWithError()
{
    printf("Nespravny vstup.\n");
    return 0;
}

int hour1, hour2;
int min1, min2;
int sec1, sec2;
int ms1, ms2;

int hour, min, sec, ms;

int main(void)
{

    char msString1[3] = {'\0', '\0', '\0'};
    char msString2[3] = {'\0', '\0', '\0'};
    char end1 = '\0';
    char end2 = '\0';

    // Get input for time 1
    printf("Zadejte cas t1:\n");
    int conversions1 = scanf(" %d : %d : %d , %03s%c", &hour1, &min1, &sec1, &msString1[0], &end1);

    // Check input format
    if (conversions1 != 4 && conversions1 != 5)
        return exitWithError();

    if (msString1[0] < '0' || msString1[0] > '9')
        return exitWithError();

    if (end1 != '\n')
        return exitWithError();

    //Check what digits were entered for milliseconds
    //If nothing is entered in some place, reset to char '0'' = ASCII 48'
    if (msString1[0] < '0')
        msString1[0] = '0';

    if (msString1[1] < '0')
        msString1[1] = '0';

    if (msString1[2] < '0')
        msString1[2] = '0';

    //Convert ASCII digits to 3 digit decimal value
    ms1 = ((msString1[0] - '0') * 100) + ((msString1[1] - '0') * 10) + ((msString1[2] - '0') * 1);

    // Check limit values
    if (hour1 >= 24 || hour1 < 0)
        return exitWithError();
    if (min1 >= 60 || min1 < 0)
        return exitWithError();
    if (sec1 >= 60 || sec1 < 0)
        return exitWithError();
    if (ms1 >= 1000 || ms1 < 0)
        return exitWithError();

    // Get input for time 2
    printf("Zadejte cas t2:\n");
    int conversions2 = scanf(" %d : %d : %d , %03s%c", &hour2, &min2, &sec2, &msString2[0], &end2);

    // Check input format
    if (conversions2 != 4 && conversions2 != 5)
        return exitWithError();

    if (msString2[0] < '0' || msString2[0] > '9')
        return exitWithError();

    if (end2 != '\n')
        return exitWithError();

    //Check what digits were entered for milliseconds
    //If nothing is entered in some place, reset to char '0'' = ASCII 48'
    if (msString2[0] < '0')
        msString2[0] = '0';

    if (msString2[1] < '0')
        msString2[1] = '0';

    if (msString2[2] < '0')
        msString2[2] = '0';

    //Convert ASCII digits to 3 digit decimal value
    ms2 = ((msString2[0] - '0') * 100) + ((msString2[1] - '0') * 10) + ((msString2[2] - '0') * 1);

    // Check limit values
    if (hour2 >= 24 || hour2 < 0)
        return exitWithError();
    if (min2 >= 60 || min2 < 0)
        return exitWithError();
    if (sec2 >= 60 || sec2 < 0)
        return exitWithError();
    if (ms2 >= 1000 || ms2 < 0)
        return exitWithError();

    // Check if t1 <= t2
    if (hour1 > hour2)
        return exitWithError();
    else if (hour1 == hour2 && min1 > min2)
        return exitWithError();
    else if (hour1 == hour2 && min1 == min2 && sec1 > sec2)
        return exitWithError();
    else if (hour1 == hour2 && min1 == min2 && sec1 == sec2 && ms1 > ms2)
        return exitWithError();

    //int hour = 0;
    //int min = 0;
    //int sec = 0;
    //int ms = 0;

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

    // Output the result
    printf("Doba: %2d:%02d:%02d,%03d\n", hour, min, sec, ms);

    return 0;
}

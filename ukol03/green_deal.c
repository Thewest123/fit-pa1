#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#endif /* __PROGTEST__ */

#define ONE_DAY_CONSUMPTION 292886

/**
 * @brief Returns 1 if the year is a leap year, otherwise returns 0
 * 
 * @param year 
 * @return int 
 */
int isLeapYear(int year)
{
    return (year % 4 == 0 && ((year % 100 != 0 || year % 400 == 0) && year % 4000 != 0));
}

int getDaysInYear(int year)
{
    if (isLeapYear(year))
        return 366;
    else
        return 365;
}

int getDaysInMonth(int year, int month)
{
    int months[] = {31,
                    28,
                    31,
                    30,
                    31,
                    30,
                    31,
                    31,
                    30,
                    31,
                    30,
                    31};

    if (month == 2 && isLeapYear(year))
        return 29;

    return months[month - 1];
}

int convertToMinutes(int year, int month, int day, int hour, int minute)
{
    int result = 0;

    result += minute;
    result += hour * 60;
    result += day * 24 * 60;

    for (int i = 1; i < month; i++)
    {
        result += getDaysInMonth(year, i) * 24 * 60;
    }

    return result;
}

int countDaysFromBeggining(int year, int month, int day)
{
    int result = 0;

    result += day;

    for (int i = 1; i < month; i++)
    {
        result += getDaysInMonth(year, i);
    }

    return result;
}

int getConsumptionForChange(int digit1, int digit2)
{
    unsigned int digits[] = {
        0b0111111,
        0b0000110,
        0b1011011,
        0b1001111,
        0b1100110,
        0b1101101,
        0b1111101,
        0b0000111,
        0b1111111,
        0b1101111};

    unsigned int changedBits = digits[digit1] ^ digits[digit2];
    int changesCount = __builtin_popcount(changedBits);

    return changesCount;
}

int energyConsumption(int y1, int m1, int d1, int h1, int i1,
                      int y2, int m2, int d2, int h2, int i2, long long int *consumption)
{
    // Check input for valid numbers
    if (y1 < 1600 || y2 < 1600)
        return 0;
    if (m1 < 1 || m1 > 12 || m2 < 1 || m2 > 12)
        return 0;
    if (d1 < 1 || d1 > getDaysInMonth(y1, m1) || d2 < 1 || d2 > getDaysInMonth(y2, m2))
        return 0;
    if (h1 < 0 || h1 > 23 || h2 < 0 || h2 > 23)
        return 0;
    if (i1 < 0 || i1 > 60 || i2 < 0 || i2 > 60)
        return 0;

    if (y1 > y2)
        return 0;

    int minutes1 = convertToMinutes(y1, m1, d1, h1, i1);
    int minutes2 = convertToMinutes(y2, m2, d2, h2, i2);

    // Check that 2nd timestamp is after the 1st timestamp
    if (minutes1 > minutes2 && y1 == y2)
        return 0;

    // TODO: Extract to separate function
    // Count days between dates
    int days1 = countDaysFromBeggining(y1, m1, d1);
    int days2 = countDaysFromBeggining(y2, m2, d2);
    int daysDiff = 0;

    if (y1 != y2)
    {
        int daysToEnd = getDaysInYear(y1) - days1;
        daysDiff = daysToEnd;

        for (int i = y1 + 1; i < y2; i++)
        {
            daysDiff += getDaysInYear(i);
        }

        daysDiff += days2;
    }
    else
    {
        daysDiff = days2 - days1;
    }

    *consumption = daysDiff * ONE_DAY_CONSUMPTION;

    // TODO: Extract to separate function
    // !! REWRITE THIS FU*KIN UGLY MESS
    // Count clock consumption
    int seconds1 = (h1 * 60 * 60) + (i1 * 60);
    int seconds2 = (h2 * 60 * 60) + (i2 * 60);

    if (seconds1 < seconds2)
    {
        int hour = h1;
        int min = i1;
        int sec = 0;

        while (hour < h2 || min < i2 || sec != 0)
        {
            if (min == 59 && sec == 59)
            {
                int oldHour = hour++;
                *consumption += getConsumptionForChange(oldHour / 10, hour / 10);
                *consumption += getConsumptionForChange(oldHour % 10, hour % 10);
                *consumption += getConsumptionForChange(min / 10, 0);
                *consumption += getConsumptionForChange(min % 10, 0);
                *consumption += getConsumptionForChange(sec / 10, 0);
                *consumption += getConsumptionForChange(sec % 10, 0);
                min = 0;
                sec = 0;
                continue;
            }

            if (sec == 59)
            {
                int oldMin = min++;
                *consumption += getConsumptionForChange(oldMin / 10, min / 10);
                *consumption += getConsumptionForChange(oldMin % 10, min % 10);
                *consumption += getConsumptionForChange(sec / 10, 0);
                *consumption += getConsumptionForChange(sec % 10, 0);
                sec = 0;
                continue;
            }

            int oldSec = sec++;

            *consumption += getConsumptionForChange(oldSec / 10, sec / 10);
            *consumption += getConsumptionForChange(oldSec % 10, sec % 10);
        }
    }
    else
    {
        *consumption -= ONE_DAY_CONSUMPTION;
        int hour = h1;
        int min = i1;
        int sec = 0;

        while (hour < 23 || min < 59 || sec != 59)
        {
            if (min == 59 && sec == 59)
            {
                int oldHour = hour++;
                *consumption += getConsumptionForChange(oldHour / 10, hour / 10);
                *consumption += getConsumptionForChange(oldHour % 10, hour % 10);
                *consumption += getConsumptionForChange(min / 10, 0);
                *consumption += getConsumptionForChange(min % 10, 0);
                *consumption += getConsumptionForChange(sec / 10, 0);
                *consumption += getConsumptionForChange(sec % 10, 0);
                min = 0;
                sec = 0;
                continue;
            }

            if (sec == 59)
            {
                int oldMin = min++;
                *consumption += getConsumptionForChange(oldMin / 10, min / 10);
                *consumption += getConsumptionForChange(oldMin % 10, min % 10);
                *consumption += getConsumptionForChange(sec / 10, 0);
                *consumption += getConsumptionForChange(sec % 10, 0);
                sec = 0;
                continue;
            }

            int oldSec = sec++;

            *consumption += getConsumptionForChange(oldSec / 10, sec / 10);
            *consumption += getConsumptionForChange(oldSec % 10, sec % 10);
        }

        *consumption += getConsumptionForChange(23 / 10, 0);
        *consumption += getConsumptionForChange(23 % 10, 0);
        *consumption += getConsumptionForChange(59 / 10, 0);
        *consumption += getConsumptionForChange(59 % 10, 0);
        *consumption += getConsumptionForChange(59 / 10, 0);
        *consumption += getConsumptionForChange(59 % 10, 0);

        hour = 0;
        min = 0;
        sec = 0;

        while (hour < h2 || min < i2 || sec != 0)
        {
            if (min == 59 && sec == 59)
            {
                int oldHour = hour++;
                *consumption += getConsumptionForChange(oldHour / 10, hour / 10);
                *consumption += getConsumptionForChange(oldHour % 10, hour % 10);
                *consumption += getConsumptionForChange(min / 10, 0);
                *consumption += getConsumptionForChange(min % 10, 0);
                *consumption += getConsumptionForChange(sec / 10, 0);
                *consumption += getConsumptionForChange(sec % 10, 0);
                min = 0;
                sec = 0;
                continue;
            }

            if (sec == 59)
            {
                int oldMin = min++;
                *consumption += getConsumptionForChange(oldMin / 10, min / 10);
                *consumption += getConsumptionForChange(oldMin % 10, min % 10);
                *consumption += getConsumptionForChange(sec / 10, 0);
                *consumption += getConsumptionForChange(sec % 10, 0);
                sec = 0;
                continue;
            }

            int oldSec = sec++;

            *consumption += getConsumptionForChange(oldSec / 10, sec / 10);
            *consumption += getConsumptionForChange(oldSec % 10, sec % 10);
        }
    }

    return 1;
}

#ifndef __PROGTEST__
int main(int argc, char *argv[])
{
    long long int consumption;

    /*assert(energyConsumption(2007, 3, 2, 13, 15,
                             2012, 2, 7, 18, 45, &consumption) == 1 &&
           consumption == 67116LL);*/

    assert(energyConsumption(2021, 10, 1, 13, 15,
                             2021, 10, 1, 18, 45, &consumption) == 1 &&
           consumption == 67116LL);
    assert(energyConsumption(2021, 10, 1, 13, 15,
                             2021, 10, 2, 11, 20, &consumption) == 1 &&
           consumption == 269497LL);
    assert(energyConsumption(2021, 1, 1, 13, 15,
                             2021, 10, 5, 11, 20, &consumption) == 1 &&
           consumption == 81106033LL);
    assert(energyConsumption(2024, 1, 1, 13, 15,
                             2024, 10, 5, 11, 20, &consumption) == 1 &&
           consumption == 81398919LL);
    assert(energyConsumption(1900, 1, 1, 13, 15,
                             1900, 10, 5, 11, 20, &consumption) == 1 &&
           consumption == 81106033LL);
    assert(energyConsumption(2021, 10, 1, 0, 0,
                             2021, 10, 1, 12, 0, &consumption) == 1 &&
           consumption == 146443LL);
    assert(energyConsumption(2021, 10, 1, 0, 15,
                             2021, 10, 1, 0, 25, &consumption) == 1 &&
           consumption == 2035LL);
    assert(energyConsumption(2021, 10, 1, 12, 0,
                             2021, 10, 1, 12, 0, &consumption) == 1 &&
           consumption == 0LL);
    assert(energyConsumption(2021, 10, 1, 12, 0,
                             2021, 10, 1, 12, 1, &consumption) == 1 &&
           consumption == 204LL);
    assert(energyConsumption(2021, 11, 1, 12, 0,
                             2021, 10, 1, 12, 0, &consumption) == 0);
    assert(energyConsumption(2021, 10, 32, 12, 0,
                             2021, 11, 10, 12, 0, &consumption) == 0);
    assert(energyConsumption(2100, 2, 29, 12, 0,
                             2100, 2, 29, 12, 0, &consumption) == 0);
    assert(energyConsumption(2400, 2, 29, 12, 0,
                             2400, 2, 29, 12, 0, &consumption) == 1 &&
           consumption == 0LL);
    return 0;
}
#endif /* __PROGTEST__ */
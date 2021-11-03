#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#endif /* __PROGTEST__ */

#define ONE_DAY_CONSUMPTION 292886
#define ONE_MINUTE_CONSUMPTION 200
#define DAYS_IN_4000_YEARS 1460969 //Has 969 leap years in 4000 years --> (969 * 366 + 3031 * 365)

/**
 * @brief Returns 1 if the year is a leap year, otherwise returns 0
 * 
 * @param year 
 * @return int Value 1 if leap year, 0 if regular year
 */
int isLeapYear(int year)
{
    return (year % 4 == 0 && ((year % 100 != 0 || year % 400 == 0) && year % 4000 != 0));
}

/**
 * @brief Get count of days in a year, respecting leap years
 * 
 * @param year 
 * @return int Count of days
 */
int getDaysInYear(int year)
{
    return isLeapYear(year) ? 366 : 365;
}

/**
 * @brief Get count of days in a month, respecting leap February
 * 
 * @param year 
 * @param month 
 * @return int Count of days
 */
int getDaysInMonth(int year, int month)
{
    if (month == 2 && isLeapYear(year))
        return 29;

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

    return months[month - 1];
}

/**
 * @brief Convert month, day, hour, minute to single number (minutes)
 * 
 * @param year Year is not added to the sum, only needed for leap February
 * @param month 
 * @param day 
 * @param hour 
 * @param minute 
 * @return int Sum of minutes from the beggining of a year
 */
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

/**
 * @brief Convert day, hour, minute to single number (days)
 * 
 * @param year Year is not added to the sum, only needed for leap February
 * @param month 
 * @param day 
 * @return int Sum of days from the beggining of a year
 */
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

/**
 * @brief Get number of changed segments between two 7-segment digits
 * 
 * 1 change = 1 Joul --> changes == consumption
 * 
 * @param digit1 
 * @param digit2 
 * @return int 
 */
int getConsumptionForChange(int digit1, int digit2)
{
    // Representation of turned segments in a 7-segment digit (ordered as GFEDCBA)
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

    // XOR the digits to get only the changed bits
    // popcount to count the set(changed) bits
    int changesCount = __builtin_popcount(digits[digit1] ^ digits[digit2]);

    return changesCount;
}

/**
 * @brief Count the consumption between two provided dates (without time)
 * 
 * @param y1 Year for Time1
 * @param m1 Month for Time1
 * @param d1 Day for Time1
 * @param y2 Year for Time2
 * @param m2 Month for Time2
 * @param d2 Day for Time2
 * @return long long 
 */
long long countConsumptionBetweenDates(int y1, int m1, int d1, int y2, int m2, int d2)
{
    long long daysDiff = 0;

    if (y1 != y2)
    {
        // Remaining days to the end of the year
        daysDiff = getDaysInYear(y1) - countDaysFromBeggining(y1, m1, d1);

        if (y2 - y1 > 4000)
        {
            // Skip chunks of 4000 years, every 4000 years have 969 leap years (969 * 366 + 3031 * 365)
            long long count = (y2 - y1) / 4000;

            daysDiff += count * DAYS_IN_4000_YEARS;

            // Count for the unskippable reminder
            for (int i = y1 + 1 + (count * 4000); i < y2; i++)
            {
                daysDiff += getDaysInYear(i);
            }
        }
        else
        {
            for (int i = y1 + 1; i < y2; i++)
            {
                daysDiff += getDaysInYear(i);
            }
        }

        daysDiff += countDaysFromBeggining(y2, m2, d2);
    }
    else
    {
        daysDiff = countDaysFromBeggining(y2, m2, d2) - countDaysFromBeggining(y1, m1, d1);
    }

    return daysDiff * ONE_DAY_CONSUMPTION;
}

/**
 * @brief Count consumption between two provided times
 * 
 * @param startHour 
 * @param startMin 
 * @param endHour 
 * @param endMin 
 * @return long long 
 */
long long countConsumptionBetweenTimes(int startHour, int startMin, int endHour, int endMin)
{
    long long consumption = 0;

    int currentHour = startHour;
    int currentMin = startMin;

    while (currentHour < endHour || currentMin < endMin)
    {
        if (currentMin == 59)
        {
            int previousHour = currentHour++;
            consumption += getConsumptionForChange(previousHour / 10, currentHour / 10);
            consumption += getConsumptionForChange(previousHour % 10, currentHour % 10);
            consumption += getConsumptionForChange(currentMin / 10, 0);
            consumption += getConsumptionForChange(currentMin % 10, 0);
            consumption += ONE_MINUTE_CONSUMPTION;
            currentMin = 0;
            continue;
        }

        int previousMin = currentMin++;

        consumption += getConsumptionForChange(previousMin / 10, currentMin / 10);
        consumption += getConsumptionForChange(previousMin % 10, currentMin % 10);
        consumption += ONE_MINUTE_CONSUMPTION;
    }

    return consumption;
}

/**
 * @brief Chech the input for invalid numbers and that the 2nd timestamp is after the 1st
 * 
 * @param y1 Year for Time1
 * @param m1 Month for Time1
 * @param d1 Day for Time1
 * @param h1 Hours for Time1
 * @param i1 Minutes for Time1
 * @param y2 Year for Time2
 * @param m2 Month for Time2
 * @param d2 Day for Time2
 * @param h2 Hours for Time2
 * @param i2 Minutes for Time2
 * @return int Value 1 if the input is valid, 0 if invalid
 */
int checkInput(int y1, int m1, int d1, int h1, int i1,
               int y2, int m2, int d2, int h2, int i2)
{
    if (y1 < 1600 || y2 < 1600)
        return 0;
    if (m1 < 1 || m1 > 12 || m2 < 1 || m2 > 12)
        return 0;
    if (d1 < 1 || d1 > getDaysInMonth(y1, m1) || d2 < 1 || d2 > getDaysInMonth(y2, m2))
        return 0;
    if (h1 < 0 || h1 > 23 || h2 < 0 || h2 > 23)
        return 0;
    if (i1 < 0 || i1 > 59 || i2 < 0 || i2 > 59)
        return 0;

    // Check that second timestamp is after the first
    if (y1 > y2)
        return 0;

    // If years are the same, count minutes from beggining and compare those instead
    if (y1 == y2 &&
        convertToMinutes(y1, m1, d1, h1, i1) > convertToMinutes(y2, m2, d2, h2, i2))
        return 0;

    return 1;
}

int energyConsumption(int y1, int m1, int d1, int h1, int i1,
                      int y2, int m2, int d2, int h2, int i2, long long int *consumption)
{
    if (!checkInput(y1, m1, d1, h1, i1, y2, m2, d2, h2, i2))
        return 0;

    // Get consumption between dates (no need to check individual digit/segment changes)
    *consumption = countConsumptionBetweenDates(y1, m1, d1, y2, m2, d2);

    // Count clock consumption for remaining hours and minutes
    if ((h1 * 60) + i1 < (h2 * 60) + i2)
    {
        // If second time is after the first, simply count between them
        *consumption += countConsumptionBetweenTimes(h1, i1, h2, i2);
    }
    else
    {
        // Else if the second time is earlier then first (overlaps to next day)

        // Subrract 1 full day, we're going to add only portion of it
        *consumption -= ONE_DAY_CONSUMPTION;

        // Count from h1:i1 to 23:59
        *consumption += countConsumptionBetweenTimes(h1, i1, 23, 59);

        // Count from 23:59 to 00:00
        *consumption += getConsumptionForChange(23 / 10, 0);
        *consumption += getConsumptionForChange(23 % 10, 0);
        *consumption += getConsumptionForChange(59 / 10, 0);
        *consumption += getConsumptionForChange(59 % 10, 0);
        *consumption += ONE_MINUTE_CONSUMPTION; //Also include the seconds digits

        // Count from 00:00 to h2:i2
        *consumption += countConsumptionBetweenTimes(0, 0, h2, i2);
    }

    printf("Consumption: %lld\n", *consumption);
    return 1;
}

#ifndef __PROGTEST__
int main(int argc, char *argv[])
{
    long long int consumption;

    //Moje
    assert(energyConsumption(1600, 1, 1, 0, 0,
                             1600, 1, 1, 0, 0, &consumption) == 1 &&
           consumption == 0LL);

    assert(energyConsumption(1600, 1, 1, 0, 0,
                             1600, 1, 2, 0, 0, &consumption) == 1 &&
           consumption == 292886LL);

    assert(energyConsumption(1904, 1, 1, 0, 0,
                             2400, 12, 31, 23, 59, &consumption) == 1 &&
           consumption == 53166716711LL);

    assert(energyConsumption(1600, 1, 1, 0, 0,
                             INT_MAX, 1, 1, 0, 0, &consumption) == 1 &&
           consumption == 229725478147912252LL);

    assert(energyConsumption(1600, 1, 1, 0, 0,
                             INT_MAX, 12, 31, 23, 59, &consumption) == 1 &&
           consumption == 229725478147912252LL + 106903179LL);

    consumption = 123;
    assert(energyConsumption(INT_MAX, 1, 1, 0, 0,
                             1600, 1, 1, 0, 0, &consumption) == 0 &&
           consumption == 123);

    assert(energyConsumption(INT_MAX, 12, 31, 23, 59,
                             INT_MAX, 12, 31, 23, 59, &consumption) == 1 &&
           consumption == 0);

    // Dalsi moje
    assert(energyConsumption(1634, 1, 1, 1, 2, 1774, 11, 21, 2, 60, &consumption) == 0);

    assert(energyConsumption(1234, 1, 1, 1, 2, 1774, 11, 21, 2, 1, &consumption) == 0);
    assert(energyConsumption(1610, 1, 1, 1, 2, 1774, 21, 21, 2, 1, &consumption) == 0);
    assert(energyConsumption(1610, 1, 1, 1, 2, 1774, 0, 21, 2, 1, &consumption) == 0);
    assert(energyConsumption(1610, 1, 1, 1, 2, 1774, 11, 0, 2, 1, &consumption) == 0);
    assert(energyConsumption(-3, 1, 1, 1, 2, 1774, 11, 0, 2, 1, &consumption) == 0);
    assert(energyConsumption(1610, 12, 31, 23, 58, 1610, 12, 31, 23, 59, &consumption) == 1 && consumption == 201LL);

    /* Nuly */
    assert(energyConsumption(1600, 0, 1, 1, 1, 1601, 1, 1, 1, 1, &consumption) == 0);
    assert(energyConsumption(1600, 0, 1, 1, 1, 1601, 1, 1, 1, 1, &consumption) == 0);
    assert(energyConsumption(1600, 1, 0, 1, 1, 1601, 1, 1, 1, 1, &consumption) == 0);
    assert(energyConsumption(1600, 1, 1, 0, 1, 1601, 1, 1, 1, 1, &consumption) == 1);
    assert(energyConsumption(1600, 1, 1, 1, 0, 1601, 1, 1, 1, 1, &consumption) == 1);
    assert(energyConsumption(1600, 1, 1, 1, 1, 1601, 0, 1, 1, 1, &consumption) == 0);
    assert(energyConsumption(1600, 1, 1, 1, 1, 1601, 1, 0, 1, 1, &consumption) == 0);
    assert(energyConsumption(1600, 1, 1, 1, 1, 1601, 1, 1, 0, 1, &consumption) == 1);
    assert(energyConsumption(1600, 1, 1, 1, 1, 1601, 1, 1, 1, 0, &consumption) == 1);

    /* Max pole*/
    assert(energyConsumption(1600, 12, 1, 1, 1, 1601, 1, 1, 1, 1, &consumption) == 1);
    assert(energyConsumption(1600, 1, 31, 1, 1, 1601, 1, 1, 1, 1, &consumption) == 1);
    assert(energyConsumption(1600, 1, 1, 23, 1, 1601, 1, 1, 1, 1, &consumption) == 1);
    assert(energyConsumption(1600, 1, 1, 1, 59, 1601, 1, 1, 1, 1, &consumption) == 1);
    assert(energyConsumption(1600, 1, 1, 1, 1, 1601, 12, 1, 1, 1, &consumption) == 1);
    assert(energyConsumption(1600, 1, 1, 1, 1, 1601, 1, 31, 1, 1, &consumption) == 1);
    assert(energyConsumption(1600, 1, 1, 1, 1, 1601, 1, 1, 23, 1, &consumption) == 1);
    assert(energyConsumption(1600, 1, 1, 1, 1, 1601, 1, 1, 1, 59, &consumption) == 1);

    assert(energyConsumption(1600, INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX, &consumption) == 0);

    // ProgTest
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
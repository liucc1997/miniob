#include "date.h"
#include <stdio.h>
#define is_leapyear(years) ((years % 100 != 0 && years % 4 == 0) || (years % 100 == 0 && years % 400 == 0))
#define INVALID_DATE -1

static const int MONTH_DAYS[] =           {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static const int MONTH_DAYS_LEAP[] =      {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static const int MONTH_DAYS_ACCU[] =      {0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
static const int MONTH_DAYS_LEAP_ACCU[] = {0, 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335};

bool date_valid(int years, int months, int days) {
    if (years <= 0) false;
    if (months <= 0 || months > 12) return false;
    if (days <= 0 || days > MONTH_DAYS_LEAP[months]) return false;
    if (!is_leapyear(years) && months == 2 && days > 28) {
        return false;
    }
    return true;
}

int datetint(int years, int months, int days) {
    if (!date_valid(years, months, days)) return INVALID_DATE;
    if (is_leapyear(years)) {
        years = years - 1;
        return years * 365 + (years / 4) - (years / 100) + (years / 400) \
            + MONTH_DAYS_LEAP_ACCU[months] + days;
    }
    years = years - 1;
    return years * 365 + (years / 4) - (years / 100) + (years / 400) \
            + MONTH_DAYS_ACCU[months] + days;
}

int inttdate(int value, int& years, int& months, int& days) {
    if (value <= 0) return INVALID_DATE;
    int four_handred_year_count = 0, one_handred_year_count = 0, \
        four_year_count = 0, one_year_count = 0;
    four_handred_year_count = value / 146097;
    value = value % 146097;
    one_handred_year_count = value /  36524;
    value = value % 36524;
    four_year_count = value / 1461;
    value = value % 1461;
    one_year_count = value / 365;
    value = value % 365;
    if (value == 0) {
        years = four_handred_year_count * 400 + one_handred_year_count * 100 \
        + four_year_count * 4 + one_year_count;
        months = 12;
        days = 31;
        return 0;
    }
    years = four_handred_year_count * 400 + one_handred_year_count * 100 \
        + four_year_count * 4 + one_year_count + 1;
    // years = value / 365 + 1;
    // value = value % 365;
    // int rest_days = (years / 4) - (years / 100) + (years / 400);
    // value = value - rest_days;
    // while (value <= 0) {
    //     years = years - 1;
    //     if (is_leapyear(years)) value = value + 366;
    //     else value = value + 365;
    // }
    printf("years = %d\n", years);
    const int* month_days = NULL;
    if (is_leapyear(years)) {
        month_days = MONTH_DAYS_LEAP;
    }
    else {
        month_days = MONTH_DAYS;
    }
    for (int i = 1; i < 12; i++) {
        if (value - month_days[i] <= 0) {
            months = i;
            break;
        }
        value = value - month_days[i];
    }
    days = value;
    return 0;
}

Date::Date(int date_value): date_value_(date_value) {
    date_string_ = new char[12];
    // int years, month, days;
    // year = 
}
Date::Date(int years, int months, int days) {

}
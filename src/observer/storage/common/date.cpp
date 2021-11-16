#include "date.h"
#include <stdio.h>

bool date_valid(int years, int months, int days) {
    if (years <= 0) return false;
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
    // printf("years = %d\n", years);
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

int charstdate(const char *s, int& years, int& months, int& days) {
    int a[3] = {0, 0, 0};
    int p = 0;
    int temp = 0;
    for (int i = 0; s[i] != '\0'; i++) {
        if (s[i] == ' ') continue;
        if (s[i] == '-') {
            if (p >= 2) {
                return INVALID_DATE;
            }
            a[p++] = temp;
            temp = 0;
            continue;
        }
        if (s[i] >= '0' && s[i] <= '9') {
            temp = temp * 10 + s[i] - '0';
            continue;
        }
        return INVALID_DATE;
    }
    a[p] = temp;
    // printf("DATE TEST: a = {%d, %d, %d}\n", a[0], a[1], a[2]);
    if (!date_valid(a[0], a[1], a[2])) {
        return INVALID_DATE;
    }
    years = a[0];
    months = a[1];
    days = a[2];
    return 0;
}

Date::Date(int date_value): date_value_(date_value) {}
Date::Date(int years, int months, int days) {
    date_value_ = datetint(years, months, days);
}
Date::Date(const char *date_string) {
    int y = 0, m = 0, d = 0;
    charstdate(date_string, y, m, d);
    date_value_ = datetint(y, m, d);
}
bool Date::isvalid() {
    return date_value_ >= 0;
}
char *Date::to_string() {
    int y, m, d;
    if (inttdate(date_value_, y, m, d) == INVALID_DATE) {
        return nullptr;
    }
    sprintf(date_string_, "%04d-%02d-%02d", y, m, d);
    return date_string_;
}

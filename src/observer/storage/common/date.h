#define INVALID_DATE     -1
#define DATA_STRING_LEN  10
#define is_leapyear(years) ((years % 100 != 0 && years % 4 == 0) || (years % 100 == 0 && years % 400 == 0))

static const int MONTH_DAYS[] =           {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static const int MONTH_DAYS_LEAP[] =      {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static const int MONTH_DAYS_ACCU[] =      {0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
static const int MONTH_DAYS_LEAP_ACCU[] = {0, 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335};

int datetint(int years, int months, int days);
int inttdate(int value, int& years, int& months, int& days);
int charstdate(const char *s, int& years, int& months, int& days);
bool date_valid(int years, int months, int days);

class Date {
    public:
    int date_value_;
    Date(int years, int months, int days);
    Date(int date_value);
    Date(const char *date_string);
    ~Date() {};
    
    char* to_string();
    bool  isvalid();
    private:
    char date_string_[11];
};
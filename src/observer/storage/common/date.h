int datetint(int years, int months, int days);
int inttdate(int value, int& years, int& months, int& days);
bool date_valid(int years, int months, int days);

class Date {
    public:
    int date_value_;
    char *date_string_;
    Date(int date_value);
    Date(char *date_string);
    Date(int years, int months, int days);
    ~Date();
    char* to_string();
};
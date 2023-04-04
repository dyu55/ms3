#define _CRT_SECURE_NO_WARNINGS
#include "Date.h"
#include <ctime>
#include <iomanip>
using namespace std;
namespace sdds {
    int Date::daysOfMonth(int year, int month) const {
        int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, -1 };
        int mon = month >= 1 && month <= 12 ? month : 13;
        mon--;
        return days[mon] + int((mon == 1) * ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0));
    }

    void Date::validate() {
        error1.clear();

        if (year < 1900 || year > 2100) {
            error1 = "Invalid Year";
        }
        else if (month < 1 || month > 12) {
            error1 = "Invalid Month";
        }
        else if (day < 1 || day > daysOfMonth(year, month)) {
            error1 = "Invalid Day";
        }
        else if (!dateonly) {
            if (hour < 0 || hour > 23) {
                error1 = "Invalid Hour";
            }
            else if (minute < 0 || minute > 59) {
                error1 = "Invlid Minute";
            }
        }

    }


    
    void getSystemDate(int& year, int& mon, int& day, int& hour, int& min, bool dateOnly) {
        time_t t = time(NULL);
        tm lt = *localtime(&t);
        day = lt.tm_mday;
        mon = lt.tm_mon + 1;
        year = lt.tm_year + 1900;
        if (dateOnly) {
            hour = min = 0;
        }
        else {
            hour = lt.tm_hour;
            min = lt.tm_min;
        }
    }

    Date::Date() {
        getSystemDate(year, month, day, hour, minute, false);
        dateonly = false;
        validate();
    }
    Date::Date(bool dateOnlyFlag) {
    getSystemDate(year, month, day, hour, minute, dateOnlyFlag);
    dateonly = dateOnlyFlag;
    validate();
}

    Date::Date(int year, int month, int day)
        : year(year), month(month), day(day), hour(0), minute(0){
        dateonly = true;
        validate();
    }
    Date::Date(int year, int month, int day, int hour)
        : year(year), month(month), day(day), hour(hour), minute(0) {
         dateonly = false;
        validate();
    }


    Date::Date(int year, int month, int day, int hour, int minute)
        : year(year), month(month), day(day), hour(hour), minute(minute) {
         dateonly = false;
        validate();
    }

    bool Date::operator==(const Date& other) const {
        return year == other.year && month == other.month && day == other.day &&
            (dateonly || (hour == other.hour && minute == other.minute));
    }

    bool Date::operator!=(const Date& other) const {
        return !(*this == other);
    }

    bool Date::operator<(const Date& other) const {
        bool result = false;

        if (year != other.year) {
            result = year < other.year;
        }
        else if (month != other.month) {
            result = month < other.month;
        }
        else if (day != other.day) {
            result = day < other.day;
        }
        else if (!dateonly) {
            if (hour != other.hour) {
                result = hour < other.hour;
            }
            else {
                result = minute < other.minute;
            }
        }

        return result;
    }

    bool Date::operator>(const Date& other) const {
        return other < *this;
    }

    bool Date::operator<=(const Date& other) const {
        return !(*this > other);
    }

    bool Date::operator>=(const Date& other) const {
        return !(*this < other);
    }

    Date& Date::dateOnly(bool flag) {
        dateonly = flag;
        if (flag) {
            hour = minute = 0;
        }
        return *this;
    }

    Date::operator bool() const {
        return !error1;
    }

    const Error& Date::error() const {
        return error1;
    }

    std::ostream& operator<<(std::ostream& os, const Date& date) {
        if (!date) {
            os << date.error() << "(";
        }
        os << date.year << '/';
            os <<setfill('0')
            << std::setw(2) << date.month << '/'
            << std::setw(2) << date.day;
        if (!date.dateonly) {
            os << ", " << std::setw(2) << date.hour << ':'
                << std::setw(2) << date.minute;
        }
        if (!date) {
            os << ")";
        }
        return os;
    }
   
    std::istream& operator>>(std::istream& is, Date& date) {
        bool inputSuccess = true;
        date.error1.clear();
        date.year = date.month = date.day = date.hour = date.minute = 0;

        if (!(is >> date.year)) {
            date.error1 = "Cannot read year entry";
            inputSuccess = false;
        }
        is.ignore();

        if (inputSuccess && !(is >> date.month)) {
            date.error1 = "Cannot read month entry";
            inputSuccess = false;
        }
        is.ignore();

        if (inputSuccess && !(is >> date.day)) {
            date.error1 = "Cannot read day entry";
            inputSuccess = false;
        }

        if (inputSuccess && !date.dateonly) {
            is.ignore();
            if (!(is >> date.hour)) {
                date.error1 = "Cannot read hour entry";
                inputSuccess = false;
            }
            is.ignore();

            if (inputSuccess && !(is >> date.minute)) {
                date.error1 = "Cannot read minute entry";
                inputSuccess = false;
            }
        }
        else {
            date.hour = date.minute = 0;
        }

        if (inputSuccess) {
            date.validate();
        }

        return is;
    }



}

#define _CRT_SECURE_NO_WARNINGS

#ifndef DATE_H
#define DATE_H

#include "Error.h"
#include <iostream>
namespace sdds {
    class Date {
    private:
        int year, month, day, hour, minute;
        bool dateonly;
        Error error1;

        int daysOfMonth(int year, int month) const;
        void validate();

    public:
        Date();
        Date(bool dateOnlyFlag);
        Date(int year, int month, int day);
        Date(int year, int month, int day, int hour);
        Date(int year, int month, int day, int hour, int minute);

        bool operator==(const Date& other) const;
        bool operator!=(const Date& other) const;
        bool operator<(const Date& other) const;
        bool operator>(const Date& other) const;
        bool operator<=(const Date& other) const;
        bool operator>=(const Date& other) const;

        Date& dateOnly(bool flag);

        operator bool() const;
        const Error& error() const;
        
        friend std::ostream& operator<<(std::ostream& os, const Date& date);
        friend std::istream& operator>>(std::istream& is, Date& date);
    };
}


#endif // DATE_H

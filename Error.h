#define _CRT_SECURE_NO_WARNINGS

#ifndef ERROR_H
#define ERROR_H

#include <iostream>
namespace sdds {
    class Error {
    private:
        char* errorMessage;
        void clearMessage();

    public:
        Error();
        Error(const char* message);
        Error(const Error& other);
        Error& operator=(const Error& other);
        ~Error();

        Error& operator=(const char* message);
        operator bool() const;
        Error& clear();

        friend std::ostream& operator<<(std::ostream& os, const Error& err);
    };
}


#endif // ERROR_H

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

#include "Error.h"
#include <cstring>
using namespace std;
namespace sdds {
    void Error::clearMessage() {
        if (errorMessage != nullptr) {
            delete[] errorMessage;
            errorMessage = nullptr;
        }
    }

    Error::Error() : errorMessage(nullptr) {}

    Error::Error(const char* message) {
        if (message == nullptr || message[0] == '\0') {
            errorMessage = nullptr;
        }
        else {
            errorMessage = new char[strlen(message) + 1];
            strcpy(errorMessage, message);
        }
    }

    Error::Error(const Error& other) {
        if (other.errorMessage == nullptr || other.errorMessage[0] == '\0') { // This line has been modified
            errorMessage = nullptr;
        }
        else {
            errorMessage = new char[strlen(other.errorMessage) + 1];
            strcpy(errorMessage, other.errorMessage);
        }
    }


    Error& Error::operator=(const Error& other) {
        if (this != &other) {
            clearMessage();
            if (other.errorMessage != nullptr) {
                errorMessage = new char[strlen(other.errorMessage) + 1];
                strcpy(errorMessage, other.errorMessage);
            }
        }
        return *this;
    }

    Error::~Error() {
        clearMessage();
    }

    Error& Error::operator=(const char* message) {
        clearMessage();
        if (message != nullptr && message[0] != '\0') {
            errorMessage = new char[strlen(message) + 1];
            strcpy(errorMessage, message);
        }
        return *this;
    }

    Error::operator bool() const {
        return errorMessage != nullptr;
    }

    Error& Error::clear() {
        clearMessage();
        return *this;
    }

    std::ostream& operator<<(std::ostream& os, const Error& err) {
        if (err.errorMessage != nullptr) {
            os << err.errorMessage;
        }
        return os;
    }
}


#define _CRT_SECURE_NO_WARNINGS

#ifndef POSIO_H
#define POSIO_H

#include <iostream>
#include <fstream>
#include "POS.h"
namespace sdds {
    class PosIO {
    public:
        virtual std::ostream& write(std::ostream& os) const = 0;
        virtual std::istream& read(std::istream& is) = 0;
        virtual std::ofstream& save(std::ofstream& ofs) const = 0;
        virtual std::ifstream& load(std::ifstream& ifs) = 0;
        virtual ~PosIO() = default;
    };

    std::ostream& operator<<(std::ostream& os, const PosIO& obj);
    std::istream& operator>>(std::istream& is, PosIO& obj);
    std::ofstream& operator<<(std::ofstream& ofs, const PosIO& obj);
    std::ifstream& operator>>(std::ifstream& ifs, PosIO& obj);

}

#endif // POSIO_H

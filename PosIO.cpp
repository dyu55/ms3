#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "PosIO.h"
using namespace std;
namespace sdds {
    std::ostream& operator<<(std::ostream& os, const PosIO& obj) {
        return obj.write(os);
    }

    std::istream& operator>>(std::istream& is, PosIO& obj) {
        return obj.read(is);
    }

    std::ofstream& operator<<(std::ofstream& ofs, const PosIO& obj) {
        return obj.save(ofs);
    }

    std::ifstream& operator>>(std::ifstream& ifs, PosIO& obj) {
        return obj.load(ifs);
    }
}


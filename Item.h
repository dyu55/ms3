#define _CRT_SECURE_NO_WARNINGS

#ifndef ITEM_H
#define ITEM_H

#include <iostream>
#include <fstream>
#include "Error.h"
#include "PosIO.h"
#include "POS.h"
namespace sdds {
    class Item : public PosIO {
    private:
        char SKU[MAX_SKU_LEN + 1];
        char* Name;
        double Price;
        bool Taxed;
        int Quantity;
        int displayTYpe;
        Error errorState;

    public:
        Item();
        Item(const Item& other);
        Item& operator=(const Item& other);
        ~Item();

        bool operator==(const Item& rhs) const;
        bool operator>(const Item& rhs) const;
        bool operator==(const char* sku) const;

     
        int operator+=(int value) ;
        int operator-=(int value) ;
        operator bool() const;

        friend double& operator+=(double& left, const Item& right);

        virtual char itemType() const;

        Item& displayType(int value);

        double cost() const;
        int quantity() const;
        Item& clear();
        const char* getSKU() const;

        std::ostream& write(std::ostream& os) const override;
        std::istream& read(std::istream& is) override;
        std::ofstream& save(std::ofstream& ofs) const override;
        std::ifstream& load(std::ifstream& ifs) override;

        std::ostream& bprint(std::ostream& os) const;
    };
}


#endif // ITEM_H

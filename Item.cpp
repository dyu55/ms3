#define _CRT_SECURE_NO_WARNINGS
#include "Item.h"
#include "Error.h"
#include <string>

#include <cstring>
#include <iomanip>
using namespace std;
namespace sdds {

    Item::Item() : SKU{ 0 }, Name{ nullptr }, Price{ 0.0 }, Taxed{ false }, Quantity{ 0 }, displayTYpe{ POS_LIST } {
        // Empty state constructor
    }

    Item::Item(const Item& other) {
        *this = other;
    }

    Item& Item::operator=(const Item& other) {
        if (this != &other) {
            strncpy(SKU, other.SKU, MAX_SKU_LEN);
            SKU[MAX_SKU_LEN] = '\0';

            delete[] Name;

            if (other.Name != nullptr) {
                Name = new char[strlen(other.Name) + 1];
                strcpy(Name, other.Name);
            }
            else {
                Name = nullptr;
            }

            Price = other.Price;
            Taxed = other.Taxed;
            Quantity = other.Quantity;
            displayTYpe = other.displayTYpe;
            errorState = other.errorState;
        }
        return *this;
    }

    Item::~Item() {
        delete[] Name;
    }

    bool Item::operator==(const Item& rhs) const {
        return itemType() == rhs.itemType() && strcmp(getSKU(), rhs.getSKU()) == 0;
    }
    bool Item::operator==(const char* sku) const {
        return strcmp(getSKU(), sku) == 0 && itemType() == 'T';
    }


    bool Item::operator>(const Item& rhs) const {
        return strcmp(Name, rhs.Name) > 0;
    }


    int Item::operator+=(int value) {
        Quantity += value;
        if (Quantity > MAX_STOCK_NUMBER) {
            Quantity = MAX_STOCK_NUMBER;
        }
        return Quantity;
    }

    int Item::operator-=(int value) {
        Quantity -= value;
        if (Quantity < 0) {
            Quantity = 0;
        }
        return Quantity;
    }
    Item::operator bool() const {
        return !errorState;
    }

    double& operator+=(double& left, const Item& right) {
        left += right.cost() * right.quantity();
        return left;
    }

    Item& Item::displayType(int value) {
        displayTYpe = value;
        return *this;
    }

    double Item::cost() const {
        return Taxed ? Price * 1.13 : Price;
    }

    int Item::quantity() const {
        return Quantity;
    }
    char Item::itemType() const {
        return 'I';
    }

    Item& Item::clear() {
        errorState.clear();
        return *this;
    }
    ostream& Item::write(ostream& os) const {

        if (*this) {
            if (displayTYpe == POS_LIST) {
                string nameStr(Name ? Name : "");

                os << left << setw(MAX_SKU_LEN) << SKU << "|"
                    << setw(20) << nameStr.substr(0, 20) << "|"
                    << setw(7) << right << fixed << setprecision(2) << Price << "|"
                    << (Taxed ? " X |" : "   |")
                    << setw(4) << right << Quantity << "|"
                    << setw(9) << right << fixed << setprecision(2) << cost() * Quantity << "|";
            }
            else if (displayTYpe == POS_FORM) {
                os << "=============v" << std::endl;
                os << "Name:        " << Name << std::endl;
                os << "Sku:         " << SKU << std::endl;
                os << "Price:       " << std::fixed << std::setprecision(2) << Price << std::endl;
                os << "Price + tax: ";
                if (Taxed) {
                    os << std::fixed << std::setprecision(2) << cost();
                }
                else {
                    os << "N/A";
                }
                os << endl;
                os << "Stock Qty:   " << Quantity << endl;
            }
        }
        else {
            os << errorState;
        }
        return os;
    }
    const char* Item::getSKU() const {
        return SKU;
    }
    istream& Item::read(istream& is) {
        char sku[MAX_SKU_LEN + 2];
        char name[MAX_NAME_LEN + 1];
        double price;
        char tax;
        int qty;
        bool skuError, nameError, priceError, taxError, qtyError;
        cout << "Sku\n> ";
        do {
            is.getline(sku, MAX_SKU_LEN);
            skuError = is.fail() || strlen(sku) > MAX_SKU_LEN;
            if (skuError) {
                is.clear();
                is.ignore(1000, '\n');
                cout << "SKU too long" << "\n> ";
            }
        } while (skuError);
        cout << "Name\n> ";
        do {
            is.getline(name, MAX_NAME_LEN);
            nameError = is.fail() || strlen(name) > MAX_NAME_LEN;
            if (nameError) {
                is.clear();
                is.ignore(1000, '\n');
                cout << "Item name too long" << "\n> ";
            }
        } while (nameError);
        cout << "Price\n> ";
        do {
            is >> price;
            priceError = is.fail() || price < 0;
            if (priceError) {
                is.clear();
                is.ignore(1000, '\n');
                cout << "Invalid price value" << "\n> ";
            }
        } while (priceError);
        cout << "Taxed?\n(Y)es/(N)o: ";
        do {
            is >> tax;
            taxError = is.fail() || (tax != 'Y' && tax != 'y' && tax != 'N' && tax != 'n');
            if (taxError) {
                is.clear();
                is.ignore(1000, '\n');
                cout << "Only 'y' and 'n' are acceptable: ";
            }
        } while (taxError);
        is.ignore(1000, '\n');
        cout << "Quantity\n> ";
        do {
            is >> qty;
            qtyError = is.fail() || qty <= 0 || qty >= MAX_STOCK_NUMBER;
            if (qtyError) {
                is.clear();
                is.ignore(1000, '\n');
                cout << "Invalid quantity value" << "\n> ";
            }
        } while (qtyError);
        is.ignore(1000, '\n');
        strcpy(SKU, sku);
        delete[] Name;
        Name = new char[strlen(name) + 1];
        strcpy(Name, name);
        Price = price;
        Taxed = (tax == 'Y' || tax == 'y');
        Quantity = qty;
        return is;
    }



    ofstream& Item::save(ofstream& ofs) const {
        if(*this) {
            ofs << itemType() << ',' << SKU << ',' << Name << ',' << fixed << setprecision(2) << Price << ',' << (Taxed ? '1' : '0') << ',' << Quantity;
        }
        else {
            cerr << errorState << endl;
        }
        return ofs;
    }

    ifstream& Item::load(ifstream& ifstr)
    {
        errorState.clear();

        char sku[256];
        char name[256];
        double price;
        int taxed;
        int quantity;
        ifstr.get(sku, 256, ',');
        ifstr.get();
        ifstr.get(name, 256, ',');
        ifstr.get();
        ifstr >> price;
        ifstr.get();
        ifstr >> taxed;
        ifstr.get();
        ifstr >> quantity;

        if (!ifstr.fail())
        {
            if (strlen(sku) > MAX_SKU_LEN)
            {
                errorState = ERROR_POS_SKU;
            }
            else
            {
                if (strlen(name) > MAX_NAME_LEN)
                {
                    errorState = ERROR_POS_NAME;
                }
                else
                {
                    if (price < 1.0)
                    {
                        errorState = ERROR_POS_PRICE;
                    }
                    else
                    {
                        if (taxed != 0 && taxed != 1)
                        {
                            errorState = ERROR_POS_TAX;
                        }
                        else
                        {
                            if (quantity > MAX_STOCK_NUMBER)
                            {
                                errorState = ERROR_POS_QTY;
                            }
                            else
                            {
                                delete[] Name;
                                strcpy(SKU, sku);
                                Name = new char[strlen(name) + 1];
                                strcpy(Name, name);
                                Price = price;
                                Taxed = taxed;
                                Quantity = quantity;
                            }
                        }
                    }
                }
            }
        }
        return ifstr;
    }


    ostream& Item::bprint(ostream& os) const {
        string nameStr(Name);
        os << "| " << left << setw(20) << nameStr.substr(0, 20) << "|" << right << setw(10) << fixed << setprecision(2) << cost() << " | " << (Taxed ? " T " : "   ") << " |" << endl;        return os;
    }

}

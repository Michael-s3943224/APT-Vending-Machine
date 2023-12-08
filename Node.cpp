#include "Node.h"
#include <iostream>

//====PRICE=====
Price::Price(): dollars(0), cents(0) {};

Price::Price(unsigned int dollars, unsigned int cents) {
    //check that number of cents cannot still be converted to a dollar
    if (cents >= ONE_DOLLAR_VAL)
    {
        throw std::runtime_error("Cents cant be >= 100 for Price");
    }
    this->dollars = dollars;
    this->cents = cents;
};

unsigned int Price::getDollars() const { return dollars; }
unsigned int Price::getCents() const { return cents; }
unsigned int Price::getValue() const { return  ONE_DOLLAR_VAL * dollars + cents; }

std::string Price::getString(bool includeSign) const
{
    //get the string of the price, wehre we can choose to include the dollar sign or not
    //ASSUMPTION: we didn't modify cents to be 100 or larger else where
    std::string result = "";
    if (includeSign) {
        result += "$ "; 
    }
    result += std::to_string(dollars) + "." + ((cents < TEN_CENTS_VAL) ? "0" : "") + std::to_string(cents);
    return result;
}

std::ostream& operator <<(std::ostream& os, const Price& price)
{
    os << "Price(" << price.getString() << ")";
    return os;
}

//====STOCK=====
Stock::Stock(): id("I0000"), name(""), description(""), price(Price()), on_hand(DEFAULT_STOCK_LEVEL) {};
Stock::Stock(const std::string& id, const std::string& n, const std::string& d, const Price& p, unsigned int h):
    id(id), name(n), description(d), price(p), on_hand(h) {};

std::string Stock::getId() const { return id; }
std::string Stock::getName() const { return name; }
std::string Stock::getDescription() const { return description; }

std::string Stock::getShortDescription() const {
    //check if the description string is long enough to be shorted
    //if it is, then shorten it
    std::string result = "";
    if (description.length() <= STOCK_SHORT_DESC_LEN) {
        result = description;
    } else {
        std::string left = description.substr(0, STOCK_SHORT_DESC_LEFT_LEN);
        std::string ellipsis = std::string(STOCK_SHORT_ELLIPIS_LEN, '.');
        std::string right = description.substr(description.length() - STOCK_SHORT_DESC_RIGHT_LEN);
        result = left + ellipsis + right;
    }
    return result;
}
Price Stock::getPrice() const { return price; }
unsigned int Stock::getOnHand() const { return on_hand; }

void Stock::setOnHand(unsigned int numHand) {
    on_hand = numHand;
}

void Stock::removeOnHand(unsigned int amount)
{
    if (amount > on_hand)
    {
        throw std::runtime_error("Cannot remove stock more than what is on hand");
    }

    on_hand -= amount;
}

std::ostream& operator<<(std::ostream& os, const Stock& stock)
{
    os << "Stock(" << stock.getId() << "," << stock.getName() << "," << stock.getShortDescription() << "," << stock.getPrice() << "," << stock.getOnHand() << ")";
    return  os;
}

//====NODE=====
Node::Node(): data(nullptr), next(nullptr) {};
Node::Node(const Stock& data, Node* next): data(new Stock(data)), next(next) {}
Node::~Node(){ delete data; };
Stock& Node::getData() { return *data; }
Node* Node::getNext() const { return next; }
void Node::setNext(Node* node) { next = node; } 
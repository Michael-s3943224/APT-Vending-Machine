#ifndef NODE_H
#define NODE_H
#include <string> 
#include "Coin.h"

//the id range for stock
#define STOCK_MIN_ID 1
#define STOCK_MAX_ID 9999

//prefix for stock id
#define STOCK_ID_PREFIX 'I'

//delimiter for stock attributes when we loading it in
#define STOCK_DELIM "|"

//number of attributes for stock when we loading it in
#define STOCK_ATTRIB 5

//settings for getting the shorten description of stock
#define STOCK_SHORT_DESC_LEFT_LEN 14
#define STOCK_SHORT_DESC_RIGHT_LEN 10
#define STOCK_SHORT_ELLIPIS_LEN 3
#define STOCK_SHORT_DESC_LEN ((STOCK_SHORT_DESC_LEFT_LEN) + (STOCK_SHORT_ELLIPIS_LEN) + (STOCK_SHORT_DESC_RIGHT_LEN))

//The length of the id string not counting the nul terminator
#define IDLEN 5

//The maximum length of a product name not counting the nul terminator
#define NAMELEN 40

//The maximum length of a product description not counting the nul terminator.
#define DESCLEN 255

//The minimum length of name or description
#define MINLEN 1

//The default coin level to reset the coins to on request
#define DEFAULT_COIN_COUNT 20

//The default stock level that all new stock should start at and that we should reset to on restock
#define DEFAULT_STOCK_LEVEL 20

//The number of denominations of currency available in the system 
#define NUM_DENOMS 8

/**
 * a structure to represent a price. One of the problems with the floating
 * point formats in C++ like float and double is that they have minor issues
 * of inaccuracy due to rounding. In the case of currency this really is
 * not acceptable so we introduce our own type to keep track of currency.
 **/
class Price
{
public:
    // The dollar value, and the cents value for some price
    unsigned dollars, cents;

    //constructors and destructors
    Price();
    Price(unsigned int dollars, unsigned int cents);

    //getters and setters
    unsigned int getDollars() const;
    unsigned int getCents() const;
    unsigned int getValue() const;
    std::string getString(bool includeSign = true) const;

    //lets us std::cout this object, just for debugging
    friend std::ostream& operator<<(std::ostream& os, const Price& stock);
};

/**
 * data structure to represent a stock item within the system
 **/
class Stock
{
public:
    //the unique id for this item
    std::string id;

    //the name of this item
    std::string name;
    
    //the description of this item   
    std::string description;
    
    //the price of this item
    Price price;
    
    // how many of this item do we have on hand? 
    unsigned on_hand;    

    //constructors and destructors
    Stock();
    Stock(const std::string& id, const std::string& n, const std::string& d, const Price& p, unsigned int h);

    //getter and setters
    std::string getId() const;
    std::string getName() const;
    std::string getDescription() const;
    std::string getShortDescription() const;
    Price getPrice() const;
    unsigned int getOnHand() const;
    void setOnHand(unsigned int numHand);
    void removeOnHand(unsigned int amount);
    
    //lets us std::cout this object, just for debugging
    friend std::ostream& operator<<(std::ostream& os, const Stock& stock);

};

/**
 * the node that holds the data about an item stored in memory
 **/
class Node
{
public:
    //constructors and destructors
    Node();
    Node(const Stock& data, Node* next);
    ~Node();

    // pointer to the data held for the node 
    Stock* data;
    // pointer to the next node in the list 
    Node* next;

    //getter and setterss
    Stock& getData();
    Node* getNext() const;
    void setNext(Node* node);
};

#endif // NODE_H
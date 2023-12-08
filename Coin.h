#ifndef COIN_H
#define COIN_H

#include <stdexcept>
#include <unordered_set>

// Coin.h defines the coin structure for managing currency in the system. 
#define DELIM ","  // delimiter 

// enumeration representing the various types of currency available in the system. 
enum Denomination
{
    FIVE_CENTS, TEN_CENTS, TWENTY_CENTS, FIFTY_CENTS, ONE_DOLLAR, 
    TWO_DOLLARS, FIVE_DOLLARS, TEN_DOLLARS
};

// the number of attributes when loading the coin file
#define COIN_ATTRIB 2

//each of the denomination values
#define FIVE_CENTS_VAL 5
#define TEN_CENTS_VAL 10
#define TWENTY_CENTS_VAL 20
#define FIFTY_CENTS_VAL 50
#define ONE_DOLLAR_VAL 100
#define TWO_DOLLARS_VAL 200
#define FIVE_DOLLARS_VAL 500
#define TEN_DOLLARS_VAL 1000

// represents a coin type stored in the cash register perhaps. Each demonination
// will have exactly one of these in the cash register.
class Coin
{
public:
    // static variable to hold all the Denomation enum values
    const static std::unordered_set<Denomination> allDenom;

    // the denomination type
    enum Denomination denom;
    
    // the count of how many of these are in the cash register
    unsigned count;

    // constructor
    Coin(Denomination denom, unsigned int count);

    // getters and setters
    void setCoinCount(unsigned int amount);
    void removeCoinCount(unsigned int amount);
    void addCoinCount(unsigned int amount);

    Denomination getDenom() const;
    unsigned int getCount() const;

};

#endif // COIN_H

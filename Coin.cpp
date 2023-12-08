#include "Coin.h"
 
// all the denomination enum values
const std::unordered_set<Denomination> Coin::allDenom = {
    FIVE_CENTS, TEN_CENTS, TWENTY_CENTS, FIFTY_CENTS, ONE_DOLLAR,
    TWO_DOLLARS, FIVE_DOLLARS, TEN_DOLLARS
};

// implement functions for managing coins; this may depend on your design.
Coin::Coin(Denomination denom, unsigned int count): denom(denom), count(count) {};

void Coin::addCoinCount(unsigned int amount)
{
    this->count += amount;
}

void Coin::removeCoinCount(unsigned int amount)
{
    if (amount > count)
    {
        throw std::runtime_error("Cannot remove coins more than coin count");
    }

    this->count -= amount;
}

void Coin::setCoinCount(unsigned int amount)
{
    this->count = amount;
}

Denomination Coin::getDenom() const
{
    return this->denom;
}

unsigned int Coin::getCount() const
{
    return this->count;
}

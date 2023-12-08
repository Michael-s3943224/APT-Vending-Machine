#ifndef VENDING_MACHINE_H
#define VENDING_MACHINE

#include <iostream>
#include <algorithm>
#include <iomanip>
#include <unordered_map>
#include "LinkedList.h"
#include "Helper.h"

class VendingMachine
{
    private:
        // linked list to store the stock items
        LinkedList stockList;

        // coin list to store the denominations and their quantity
        std::vector<Coin> coinList;

        /**
         * @brief Keep prompting user until they enter a valid item ID or terminate with ^D or Enter
         * @param prompt Prompt to keep asking until success or terminatation
         * @return item ID index in stockList
         * @throws std::runtime_error
        */
        unsigned int getUserItemIndexPersistent(const std::string& prompt);

        /**
         * @brief Keep prompting user until they enter a valid item name or terminate with ^D or Enter
         * @param prompt Prompt to keep asking until success or termination
         * @return item name
         * @throws std::runtime_error
        */
        std::string getUserNamePersistent(const std::string& prompt);

        /**
         * @brief Keep prompting user until they enter a valid item description or terminate with ^D or Enter
         * @param prompt Prompt to keep asking until success or termination
         * @return item description
         * @throws std::runtime_error
        */
        std::string getUserDescriptionPersistent(const std::string& prompt);

        /**
         * @brief Keep prompting user until they enter a valid price or terminate with ^D or Enter
         * @param prompt Prompt to keep asking until success or termination
         * @return price of an item
         * @throws std::runtime_error
        */
        Price getUserPricePersistent(const std::string& prompt);

        /**
         * @brief Keep prompting user until they enter a valid Denomination value or terminate with ^D or Enter
         * @param prompt Prompt to keep asking until success or termination
         * @return denomination of a coin
         * @throws std::runtime_error
        */
        Denomination getUserDenomPersistent(const std::string& prompt);

        /**
         * @brief Generate the next available ID for stock list if one exists
         * @return item id in the format I#### where # is a digit
         * @throws std::runtime_error
        */
        std::string generateNextId();

        /**
         * @brief Prompt user for item name, description and price then create an item if not terminated midway
         * @return user created item
         * @throws std::runtime_error
        */
        Stock getUserItem(const std::string& newItemId);

    public:
        VendingMachine();

        /**
         * @brief Load the stockFile and coinFile into stockList and coinList respectively (if they exist)
         * @param stockFile the directory to the stock file to be loaded
         * @param coinFile the directory to the coin file to be loaded
         * @throws std::runtime_error
        */
        void load(const std::string& stockFile, const std::string& coinFile);

        /**
         * @brief Save the stockList and coinList into a stock file and coin file
         * @param stockFile the directory of the stock file to be saved
         * @param coinFile the diretory of the coin file to be saved
         * @throws std::runtime_error
        */
        void save(const std::string& stockFile, const std::string& coinFile);

        /**
         * @brief Reset all stocks' on hand amount to the default
        */
        void resetStock();

        /**
         * @brief Reset all coins' quantity to the default
        */
        void resetCoin();

        /**
         * @brief 
         * Prompt user for item info then add it to stockList while maintaining its ascending order in terms of item name
        */
        void addUserItem();

        /**
         * @brief Prompt user for item id if stockList is not empty the remove that item from stockList
        */
        void removeUserItem();

        /**
         * @brief Display coinList info in a table
        */
        void displayCoins();

        /**
         * @brief Display stockList info (except description) in a table
        */
        void displayStock();

        /**
         * @brief 
         * Prompt user for item id and denom value until they are able to purchase the item.
         * Then give them the correct change
         * If not enough coins in coinList, the user gets back their input coins
        */
        void purchaseItem();
};
#endif
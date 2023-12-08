#ifndef HELPER_H
#define HELPER_H
#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <algorithm>
#include <unordered_set>
#include <functional>
//awkward inclusion so we can do explicit template instantiation for Price
#include "LinkedList.h"
#include "Node.h" 
#include "Coin.h"

#include <cmath>

#define WHITESPACE " \n\r\t\f\v"
#define ERROR_PREFIX "Error: "
#define ERROR_POSTFIX ". Please try again."

class Helper
{
private:
    Helper();

public:

    /**
     * @brief 
     * GOTTEN FROM ASSIGNMENT 1
     * CHECKS IF THE STRING IS A NON-NEGATIVE NUMBER
     * Check whether the given string is an integer or a float 
     * 
     * @param s The given string
     * @return true if the string is an integer or a float 
     * @return false if the string is neither an integer nor a float 
     */
    static bool isNumber(std::string s);

    /**
     * @brief 
     * GOTTEN FROM ASSIGNMENT 1
     * Split the given string given a delimeter. For instance, given the string "1,2,3" and the delimeter ",", 
     *        This string will be splited into "1","2","3", and they will be put into the vector.  
     * 
     * @param s The given string
     * @param tokens The vector containing the results
     * @param delimeter the string based on which splitting is performed
     */
    static void splitString(std::string s, std::vector<std::string>& tokens, std::string delimeter);

    /**
     * @brief Split the string then trim each of the splits
     * @param s The given string
     * @param delimeter the string based on which splitting is performed
     * @return The vector with the split values
     */
    static std::vector<std::string> splitStringAndTrim(const std::string& s, const std::string& delimiter);
    
    /**
     * @brief 
     * GOTTEN FROM ASSIGNMENT 1
     * Read the user input (a line) from keyboard
     * 
     * @return the user input.
     */
    static std::string readInput();

    /**
     * @brief Keep prompting the user for input until the validator throws no errors or the user terminates with ^D or Enter
     * @param prompt The prompt to keep asking until success or termination
     * @param validator The function to return the parsed value
     * @throws std::runtime_error
    */
    template <typename T>
    static T getUserInputPersistent(const std::string& prompt, const std::function<T(const std::string&)>& validator);

    /**
     * @brief Remove white spaces from the left side of a string
     * @param str The string to be trimmed
     * @return The trimmed string
    */
    static std::string stringTrimLeft(const std::string& str);

    /**
     * @brief Remove white spaces from the right side of a string
     * @param str The string to be trimmed
     * @return The trimmed string
    */
    static std::string stringTrimRight(const std::string& str);

    /**
     * @brief Remove white spaces from both sides of a string
     * @param str The string to be trimmed
     * @return The trimmed string
    */
    static std::string stringTrim(const std::string& str);

    /**
     * @brief Convert a string to lower case
     * @param str The string to be converted
     * @return The lower case string
    */
    static std::string stringLower(std::string str);

    /**
     * @brief Find the digit count of a number
     * @param num The number to find the digit count for
     * @return The digit count
    */
    static unsigned int digitCount(unsigned int num);

    /**
     * @brief 
     * Round a number to the nearest multiple
     * @param num The number to round
     * @param multiple The multiple to round to
     * @return The rounded integer
    */
    static unsigned int round(unsigned int num, unsigned int multiple);
    
    /**
     * @brief Try load the stock file into a list of Stock
     * @param fileName The directory to load from
     * @return List of Stock
     * @throws std::runtime_error
    */
    static std::vector<Stock> tryLoadStockFile(const std::string& fileName);

    /**
     * @brief Try load the coin file into a list of Coin
     * @param fileName The directory to load from
     * @return List of Coin
     * @throws std::runtime_error
    */
    static std::vector<Coin> tryLoadCoinsFile(const std::string& fileName);

    /**
     * @brief Save the Stock list into a file
     * @param fileName The directory to save to
     * @param stockList The Stock list to save
    */
    static void saveStockList(const std::string& fileName, const LinkedList& stockList);

    /**
     * @brief Save the Ctock list into a file
     * @param fileName The directory to save to
     * @param stockList The Coin list to save
    */
    static void saveCoinList(const std::string& fileName, const std::vector<Coin>& coinList);

    /**
     * @brief Try parse a string to an integer value
     * @param s The string to convert
     * @return The parsed integer value
     * @throws std::runtime_error
    */
    static int tryParseInt(const std::string& s);

    /**
     * @brief Try parse a string to Price object
     * @param s The string to convert
     * @return The parsed Price object
     * @throws std::runtime_error
    */
    static Price tryParsePrice(const std::string& s);

    /**
     * @brief Try parse a string to Denomination enum value
     * @param s The string to convert
     * @return The parsed Denomination enum value
     * @throws std::runtime_error
    */
    static Denomination tryParseDenom(const std::string& s);

    /**
     * @brief Try parse an integer to Denomination enum value
     * @param i The integer to convert
     * @return The parsed Denomination enum value
     * @throws std::runtime_error
    */
    static Denomination tryParseDenom(unsigned int i);

    /**
     * @brief Convert denomination value to an integer value
     * @param denom The denomination enum value
     * @return The denomination integer value
    */
    static unsigned int denomToValue(Denomination denom);

    /**
     * @brief Convert denomination value to a string value
     * @param denom The denomination enum value
     * @return The denomination string value
    */
    static std::string denomToString(Denomination denom);

    /**
     * @brief Convert denomination value to a short string value
     * @param denom The denomination enum value
     * @return The denomination short string value
    */
    static std::string denomToShortString(Denomination denom);

    /**
     * @brief Convert price integer value to Price object
     * @param value The price integer value
     * @return The Price object
    */
    static Price valueToPrice(unsigned int value);

    /**
     * @brief 
     * Try parse an string to a item id string
     * If valid just returns the input for convenience
     * @param itemId The string to convert
     * @return The parsed item id string
     * @throws std::runtime_error
    */
    static std::string tryParseItemId(const std::string& itemId);

    /**
     * @brief 
     * Try parse a string to a string with the correct size
     * If valid just returns the input for convenience
     * @param s The string to convert
     * @param fieldName The field that this string size check is for
     * @param minSize the minimum length of the string
     * @param maxSize the maximum length of the string
     * @return The parsed string with the correcy size
     * @throws std::runtime_error
    */
    static std::string tryParseStringSize(const std::string& s, const std::string& fieldName, unsigned int minSize, unsigned int maxSize);

    /**
     * @brief 
     * Try parse a string to a name string
     * If valid just returns the input for convenience
     * @param s The string to convert
     * @return The parsed name string
     * @throws std::runtime_error
    */
    static std::string tryParseName(const std::string& s);

    /**
     * @brief 
     * Try parse a string to a description string
     * If valid just returns the input for convenience
     * @param s The string to convert
     * @return The parsed description string
     * @throws std::runtime_error
    */
    static std::string tryParseDescription(const std::string& s);

    /**
     * @brief Get the total number of coins
     * @param coinsState List of denomination and quantity
     * @return The total number of coins
    */
    static unsigned int getCoinsStateSum(unsigned int coinsState[NUM_DENOMS]);

    /**
     * @brief
     * @param remaining The remaining value we need to find coins for
     * @param coinList The coin list in the vending machine (descending order?)
     * @param coinsState The current List of denomination and quantity WHICH HAS TO BE COPIED TO PREVENT SIDE EFFECTS AGAJRGJTYJAUAKTRYJTYJATFYUAJTYRJTgj
     * @param index The current index we are at for coin list
     * @param callback Once we get to index=0 and remaining=0, we get a callback of the coinsState
    */
    static void getCoinNthCombination(unsigned int remaining, const std::vector<Coin>& coinList, unsigned int coinsState[NUM_DENOMS], int index, const std::function<void(unsigned int[NUM_DENOMS])>& callback);

    /**
     * @brief 
     * Go through all possible coin combinations to find the best one for the change
     * This was harder than expected.
     * No, maximum fit does not work, $1.10 with only 1x$1, 1x50c, 3x20c would have failed when it should
     * Because it will try to go for $1 then go for 10c which doesn't exist
     * @param remaining The change
     * @param coinList The coin list assumed to be sorted in descending order (or does it still work in any order?)
     * @return definitely not a pointer because I don't want to manage that memory (gets destroyed if I don't allocate to heap)
     * @throws std::runtime_error
    */
    static std::vector<unsigned int> getBestCoinCombination(unsigned int remaining, const std::vector<Coin>& coinList);
};

#endif
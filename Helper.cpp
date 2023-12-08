#include "Helper.h"

//Helper methods
Helper::Helper(){}

bool Helper::isNumber(std::string s)
{
    //FROM ASSIGNMENT 1
    std::string::const_iterator it = s.begin();
    char dot = '.';
    int nb_dots = 0;
    while (it != s.end()) 
    {
        if (*it == dot)
        {
            nb_dots++;
            if (nb_dots>1)
            {
                break;
            }
        }   
        else if (!isdigit(*it))
        {
            break;
        } 

        ++it;
    }
    return !s.empty() && s[0] != dot && it == s.end();
}

void Helper::splitString(std::string s, std::vector<std::string>& tokens, std::string delimeter)
{
    //FROM ASSIGNMENT 1
    tokens.clear();
    char* _s = new char[s.length()+1];
    strcpy(_s, s.c_str());

    char * pch;
    pch = strtok (_s, delimeter.c_str());
    while (pch != NULL)
    {
        tokens.push_back(pch);
        pch = strtok (NULL, delimeter.c_str());
    }
    delete[] _s;
}

std::vector<std::string> Helper::splitStringAndTrim(const std::string& s, const std::string& delimeter)
{
    std::vector<std::string> tokens;
    //split the string
    splitString(s, tokens, delimeter);

    //then trim each element in the split
    for (unsigned int i = 0; i < tokens.size(); ++i)
    {
        tokens[i] = stringTrim(tokens[i]);
    }

    return tokens;
}

std::string Helper::readInput()
{
    //FROM ASSIGNMENT 1
    //but removed the cout new line
    std::string input;
    std::getline(std::cin, input);
    return input;
}

template <typename T>
T Helper::getUserInputPersistent(const std::string& prompt, const std::function<T(const std::string&)>& validator)
{
    bool quit = false;
    T result {};

    while (!quit) 
    {   
        //print prompt and get user input
        std::cout << prompt;
        std::string inputStr = Helper::readInput();

        //if EOF of Return on empty input then terminate
        if (std::cin.eof() || inputStr.empty())
        {
            if (std::cin.eof())
            {
                //this is just to make the output more bareable to look at
                std::cout << std::endl;
            }
            throw std::runtime_error("getting input from user has been terminated");
        }

        //trim the string before validating it
        inputStr = Helper::stringTrim(inputStr);
        try
        {
            result = validator(inputStr);
            //if validation was successful ext the loop
            quit = true;
        }
        catch(const std::runtime_error& e)
        {
            //if validation was unsuccessful, print the error message and repeat
            std::cout << ERROR_PREFIX << e.what() << ERROR_POSTFIX << std::endl;
        }
    }

    return result;
}
//explicit template instantiation
template unsigned int Helper::getUserInputPersistent(const std::string& prompt, const std::function<unsigned int(const std::string&)>& validator);
template std::string Helper::getUserInputPersistent(const std::string& prompt, const std::function<std::string(const std::string&)>& validator);
template Price Helper::getUserInputPersistent(const std::string& prompt, const std::function<Price(const std::string&)>& validator);
template Denomination Helper::getUserInputPersistent(const std::string& prompt, const std::function<Denomination(const std::string&)>& validator);

//Trim functions from: https://www.techiedelight.com/trim-string-cpp-remove-leading-trailing-spaces/
std::string Helper::stringTrimLeft(const std::string& str)
{
    //find the first index that is not a whitespace from left to right
    //we return the substring starting from that index to the end
    std::size_t startIndex = str.find_first_not_of(WHITESPACE);
    return (startIndex == std::string::npos) ? "" : str.substr(startIndex);

}

std::string Helper::stringTrimRight(const std::string& str)
{
    //find the first index that is not a whitespace from right to left
    //we return the substring from the start to that index
    std::size_t endIndex = str.find_last_not_of(WHITESPACE);
    return (endIndex == std::string::npos) ? "" : str.substr(0, endIndex + 1);
}

std::string Helper::stringTrim(const std::string& str)
{
    //apply both of the trims
    return stringTrimLeft(stringTrimRight(str));
}

std::string Helper::stringLower(std::string str)
{
    //https://www.geeksforgeeks.org/conversion-whole-string-uppercase-lowercase-using-stl-c/
    //param1: start of iterator copying from
    //param2: end of iterator copying from
    //param3: start of iterator to place copy
    //param4: function for transformation
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

unsigned int Helper::digitCount(unsigned int num)
{
    //floor(log(x) + 1) is the formula for getting the digit count for numbers 1 and above
    //need a special case for 0 since log(0) is undefined
    unsigned int count = 1;
    if (num != 0)
    {
        count = static_cast<unsigned int>(log10(num) + 1);
    }
    return count;
}

unsigned int Helper::round(unsigned int num, unsigned int multiple)
{
    //Taken from: https://www.geeksforgeeks.org/round-the-given-number-to-nearest-multiple-of-10/

    //round down to multiple
    unsigned int a = (num / multiple) * multiple;

    //round up to multiple
    unsigned int b = a + num;

    //check if round down or round up is closer
    return (num - a > num - b) ? b : a;
}

std::vector<Stock> Helper::tryLoadStockFile(const std::string& fileName) {
    //the output list
    std::vector<Stock> listStock;

    //used to make sure we dont have duplicate ids
    std::unordered_set<std::string> uniqueIds;

    std::ifstream fileStream;
    fileStream.open(fileName);

    //check if the file exists
    if (!fileStream) {
        throw std::runtime_error("Stock file not found");
    } 

    //get the first line of the file
    std::string line;
    std::getline(fileStream, line);
    
    //keeps track of the item number we're at
    int itemCounter = 0;

    //keep looping while the we dont reach EOF or empty line
    while (!fileStream.eof() && !line.empty()) {

        itemCounter += 1;
        std::string errorMessagePrefix = "Item No. " + std::to_string(itemCounter) + " failed, ";

        std::vector<std::string> itemInfo = Helper::splitStringAndTrim(line, STOCK_DELIM);

        //check if the number of attributes is correct
        if (itemInfo.size() != STOCK_ATTRIB) {
            fileStream.close();
            throw std::runtime_error(errorMessagePrefix + "Needs to be have " + std::to_string(STOCK_ATTRIB) + " attributes");
        }

        std::string idStr = itemInfo[0]; std::string nameStr = itemInfo[1]; std::string descStr = itemInfo[2];
        std::string priceStr = itemInfo[3]; std::string numStr = itemInfo[4];

        std::string itemId = ""; std::string name = ""; std::string desc = ""; Price price; int quantity = 0;

        //check if all the fields are correct
        try
        {
            itemId = tryParseItemId(idStr);
            if (uniqueIds.find(itemId) != uniqueIds.end()){
                throw std::runtime_error("Item Id already exists");
            }

            name = tryParseName(nameStr);
            desc = tryParseDescription(descStr);
            price = tryParsePrice(priceStr);

            try {
                quantity = tryParseInt(numStr);
            }
            catch(const std::runtime_error& e) {
                throw std::runtime_error("On hand needs to be a valid integer");
            }
        }
        catch(const std::runtime_error& e)
        {
            fileStream.close();
            throw std::runtime_error(errorMessagePrefix + std::string(e.what()));
        }

        //add the item to the stockList and add the id to the uniqueIds set
        Stock stock(itemId, name, desc, price, quantity);
        listStock.push_back(stock);
        uniqueIds.insert(idStr);

        //read the next line
        std::getline(fileStream, line);
    }

    //make sure we closed the file
    fileStream.close();

    return listStock;
} 

std::vector<Coin> Helper::tryLoadCoinsFile(const std::string& fileName) {

    //the output list
    std::vector<Coin> listCoins;

    //used to make sure we don't have duplicate denominations
    //hopefully this copies the set please
    std::unordered_set<Denomination> denomLeft = Coin::allDenom;

    std::ifstream fileStream;
    fileStream.open(fileName);

    //check if the file exists
    if (!fileStream) {
        throw std::runtime_error("Coin file not found");
    } 

    //read the first line
    std::string line;
    std::getline(fileStream, line);

    //keeps track of the coin number we're at
    int coinCounter = 0;

    //keep looping while the we dont reach EOF or empty line
    while (!fileStream.eof() && !line.empty()) {

        coinCounter += 1;
        std::string errorMessagePrefix = "Coin No. " + std::to_string(coinCounter) + " failed, ";

        std::vector<std::string> coinInfo = Helper::splitStringAndTrim(line, DELIM);

        //check if the number of attributes is correct
        if (coinInfo.size() != COIN_ATTRIB) {
            fileStream.close();
            throw std::runtime_error(errorMessagePrefix +  "Needs to be have " + std::to_string(COIN_ATTRIB) + " attributes");
        }

        std::string valueStr = coinInfo[0]; std::string numStr = coinInfo[1];

        Denomination denom = FIVE_CENTS; int quantity = 0;

        //check if all the fields are correct
        try
        {
            denom = tryParseDenom(valueStr);
            if (denomLeft.find(denom) == denomLeft.end())
            {
                throw std::runtime_error("Denomination already exists");
            }
            try{
                quantity = tryParseInt(numStr);
            }
            catch(const std::runtime_error& e){
                throw std::runtime_error("Quantity needs to be a valid integer");
            }     
        }
        catch(const std::runtime_error& e)
        {
            fileStream.close();
            throw std::runtime_error(errorMessagePrefix + std::string(e.what()));
        }

        //add the coin to the coin list and remove its Denominatio enum value from the denomLeft srt
        Coin coin(denom, quantity);
        listCoins.push_back(coin);
        denomLeft.erase(denom);
        
        //read the next line
        std::getline(fileStream, line);
    }
    
    //make sure we close the file
    fileStream.close();

    // check if the denomLeft set is empty
    // if it is empty then all the denomination values have been added
    if (!denomLeft.empty())
    {
        throw std::runtime_error("Coins File must contain all denomination values");
    }

    return listCoins;
}

void Helper::saveStockList(const std::string& fileName, const LinkedList& stockList)
{
    std::ofstream file;

    //open the file with everything cleared beforehand
    //ASSUMPTION: file already exists, or does std::fstream::trunc create it for me? Dunno, didn't check
    file.open(fileName, std::fstream::out | std::fstream::trunc);

    //loop through the stock list and shove in each item as a line of text in the file
    stockList.forEach([&file](const Stock& s)
    {
        file << s.getId() << STOCK_DELIM << s.getName() << STOCK_DELIM << s.getDescription() << STOCK_DELIM << s.getPrice().getString(false) << STOCK_DELIM << s.getOnHand() << std::endl;
    });
    
    //make sure to close the file
    file.close();
}

void Helper::saveCoinList(const std::string& fileName, const std::vector<Coin>& coinList)
{
    std::ofstream file;

    //open the file with everything cleared beforehand
    //ASSUMPTION: file already exists, or does std::fstream::trunc create it for me? Dunno, didn't check
    file.open(fileName, std::fstream::out | std::fstream::trunc);

    //loop through the coin list and shove in each coin as a line of text in the file
    for (Coin coin: coinList)
    {
        file << Helper::denomToValue(coin.getDenom()) << DELIM << coin.getCount() << std::endl;
    }

    //make sure to close the file
    file.close();
}

int Helper::tryParseInt(const std::string& s)
{
    int result = 0;
    char dot = '.';
    
    try
    {
        //check if there is no decimal point and the string represents a number
        if (s.find(dot) == std::string::npos && isNumber(s))
        {
            result = std::stoi(s);
        }
        else
        {
            throw std::runtime_error("integer incorrect format");
        }
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("integer overflow exception");
    }
    
    return result;
}

Price Helper::tryParsePrice(const std::string& s)
{   
    //find where the dot is
    char dot = '.';
    std::size_t dotIndex = s.find_last_of(dot);

    //extract the dollar string and cents string
    std::string dollarsStr = s.substr(0, dotIndex);
    std::string centsStr = s.substr(dotIndex + 1);
    std::size_t validDecimalPlaces = 2;

    //check if we found a dot, the cents has the correct number of places, everything is in the format of a number (no duplicate dots)
    if (dotIndex == std::string::npos || centsStr.length() != validDecimalPlaces || !isNumber(s))
    {
        throw std::runtime_error("Price needs to be in format #.00 (# is positive integer)");
    }

    int dollars = 0;
    int cents = 0;

    // make sure the amount of dollars doesnt overflow
    try {
        dollars = tryParseInt(dollarsStr);
    } catch (const std::runtime_error& e) {
        throw std::runtime_error("Price dollars must be a valid integer");
    }

    // probably not going to be invalid, but all well here it is
    try {
        cents = tryParseInt(centsStr);
    } catch(const std::runtime_error& e) {
        throw std::runtime_error("Price cents must be a valid integer");
    }

    //make sure the price is not for free
    if (dollars == 0 && cents == 0)
    {
        throw std::runtime_error("Price cannot be free");
    }

    //make sure the price is in multiples of the smallest denomination
    if (cents % FIVE_CENTS_VAL != 0)
    {
        throw std::runtime_error("Price cents must be divisible by 5");
    }

    return Price(dollars, cents);
}

Denomination Helper::tryParseDenom(const std::string& s)
{

    // first convert to integer
    int denomValue = FIVE_CENTS_VAL;
    try
    {
        denomValue = tryParseInt(s);
    }
    catch(const std::runtime_error& e)
    {
        throw std::runtime_error("Denomination needs to be a valid integer");
    }

    // then try to convert to Denomation from integer
    Denomination denom = FIVE_CENTS;
    try
    {
        denom = tryParseDenom(denomValue);
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error(e.what());
    }
    
    return denom;
}

Denomination Helper::tryParseDenom(unsigned int i)
{
    /*
        FIVE_CENTS      <- 5
        TEN_CENTS       <- 10
        TWENTY_CENTS    <- 20
        FIFTY_CENTS     <- 50
        ONE_DOLALR      <- 100 
        TWO_DOLALRS     <- 200
        FIVE_DOLLARS    <- 500
        TEN_DOLLARS     <- 1000

        ERROR           <- Else
    */

    Denomination result = FIVE_CENTS;

    if (i == FIVE_CENTS_VAL) {
        result = FIVE_CENTS;
    }
    else if (i == TEN_CENTS_VAL) {
        result = TEN_CENTS;
    }
    else if (i == TWENTY_CENTS_VAL) {
        result = TWENTY_CENTS;
    }
    else if (i == FIFTY_CENTS_VAL) {
        result = FIFTY_CENTS;
    }
    else if (i == ONE_DOLLAR_VAL) {
        result = ONE_DOLLAR;
    }
    else if (i == TWO_DOLLARS_VAL) {
        result = TWO_DOLLARS;
    }
    else if (i == FIVE_DOLLARS_VAL) {
        result = FIVE_DOLLARS;
    }
    else if (i == TEN_DOLLARS_VAL) {
        result = TEN_DOLLARS;
    }
    else
    {
        throw std::runtime_error("Denomination value needs to valid");
    }

    return result;
}

unsigned int Helper::denomToValue(Denomination denom)
{
    /*
        FIVE_CENTS      -> 5
        TEN_CENTS       -> 10
        TWENTY_CENTS    -> 20
        FIFTY_CENTS     -> 50
        ONE_DOLALR      -> 100 
        TWO_DOLALRS     -> 200
        FIVE_DOLLARS    -> 500
        TEN_DOLLARS     -> 1000
    */
    unsigned int result = 0;

    if (denom == FIVE_CENTS) {
        result = FIVE_CENTS_VAL;
    }
    else if (denom == TEN_CENTS) {
        result = TEN_CENTS_VAL;
    }
    else if (denom == TWENTY_CENTS) {
        result = TWENTY_CENTS_VAL;
    }
    else if (denom == FIFTY_CENTS) {
        result = FIFTY_CENTS_VAL;
    }
    else if (denom == ONE_DOLLAR) { 
        result = ONE_DOLLAR_VAL;
    }
    else if (denom == TWO_DOLLARS) {
        result = TWO_DOLLARS_VAL;
    }
    else if (denom == FIVE_DOLLARS) {
        result = FIVE_DOLLARS_VAL;
    }
    else if (denom == TEN_DOLLARS) {
        result = TEN_DOLLARS_VAL;
    }

    return result;
}

std::string Helper::denomToString(Denomination denom)
{
    /*
        FIVE_CENTS      -> 5 Cents
        TEN_CENTS       -> 10 Cents
        TWENTY_CENTS    -> 20 Cents
        FIFTY_CENTS     -> 50 Cents
        ONE_DOLALR      -> 1 Dollar
        TWO_DOLALRS     -> 2 Dollar
        FIVE_DOLLARS    -> 5 Dollar
        TEN_DOLLARS     -> 10 Dollar
    */

    unsigned int denomVal = denomToValue(denom);

    std::string result = "";

    std::string dollarStr = "Dollar";
    std::string centsStr = "Cents";
    
    if (denomVal >= ONE_DOLLAR_VAL)
    {
        result = std::to_string(denomVal / ONE_DOLLAR_VAL) + " " + dollarStr;
    }
    else
    {
        result = std::to_string(denomVal) + " " + centsStr;
    }

    return result;
}

std::string Helper::denomToShortString(Denomination denom)
{
    /*
        FIVE_CENTS      -> 5c
        TEN_CENTS       -> 10c
        TWENTY_CENTS    -> 20c
        FIFTY_CENTS     -> 50c
        ONE_DOLALR      -> $1
        TWO_DOLALRS     -> $2
        FIVE_DOLLARS    -> $5
        TEN_DOLLARS     -> $10
    */

    unsigned int denomVal = denomToValue(denom);

    std::string result = "";

    std::string dollarStr = "$";
    std::string centsStr = "c";
    
    if (denomVal >= ONE_DOLLAR_VAL)
    {
        result = dollarStr + std::to_string(denomVal / ONE_DOLLAR_VAL);
    }
    else
    {
        result = std::to_string(denomVal) + centsStr;
    }

    return result;
}

Price Helper::valueToPrice(unsigned int value)
{
    //dollars is number of times 100 goes in to value
    //cents is the remaining amount
    unsigned int dollars = value / ONE_DOLLAR_VAL;
    unsigned int cents = value % ONE_DOLLAR_VAL;
    return Price(dollars, cents);
}

std::string Helper::tryParseItemId(const std::string& itemId)
{
    std::size_t numPartStartIndex = 1;
    std::string numberPart = itemId.substr(numPartStartIndex);

    //check the length of the string
    if (itemId.length() != IDLEN)
    {   
        throw std::runtime_error("Item Id has to be length " + std::to_string(IDLEN));
    }

    //check if the prefix of the string
    if (itemId.at(0) != STOCK_ID_PREFIX)
    {
        std::string s = "Item Id needs to start with ";
        s += STOCK_ID_PREFIX;
        throw std::runtime_error(s);
    }

    //check if the last four characters is actually an integer
    int idAsInt = 0;
    try
    {
        idAsInt = tryParseInt(numberPart);
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("Item Id needs to be in format I#### (# is digit)");
    }

    //check if the number part is between STOCK_MIN_ID and STOCK_MAX_ID
    if (idAsInt < STOCK_MIN_ID || idAsInt > STOCK_MAX_ID )
    {
        throw std::runtime_error("Item Id must be between " + std::to_string(STOCK_MIN_ID) + " and " + std::to_string(STOCK_MAX_ID));
    }

    return itemId;
}

std::string Helper::tryParseStringSize(const std::string& s, const std::string& fieldName, unsigned int minSize, unsigned int maxSize)
{
    //if the string length is less the min size or more than max size, its an invalid string
    if (s.length() < minSize || s.length() > maxSize)
    {
        std::string errorMessage = 
            fieldName + " needs to be between " + std::to_string(minSize) + 
            " and " + std::to_string(maxSize) + " characters";
        
        throw std::runtime_error(errorMessage);
    }
    return s;
}

std::string Helper::tryParseName(const std::string& s)
{
    //make sure the name size is between MINLEN and NAMELEN
    std::string nameField = "Name";
    return tryParseStringSize(s, nameField, MINLEN, NAMELEN);
}

std::string Helper::tryParseDescription(const std::string& s)
{
    //make sure the description size is between MINLEN and DESCLEN
    std::string descField = "Description";
    return tryParseStringSize(s, "Description", MINLEN, DESCLEN);
}

unsigned int Helper::getCoinsStateSum(unsigned int coinsState[NUM_DENOMS])
{
    unsigned int sum = 0;
    for (unsigned int i = 0; i < NUM_DENOMS; ++i)
    {
        sum += coinsState[i];
    }
    return sum;
}

void Helper::getCoinNthCombination(unsigned int remaining, const std::vector<Coin>& coinList, unsigned int coinsState[NUM_DENOMS], int index, const std::function<void(unsigned int[NUM_DENOMS])>& callback)
{
    const Coin& coin = coinList.at(index);
    Denomination denom = coin.getDenom();
    unsigned int denomValue = Helper::denomToValue(denom);
    unsigned int perfectFit = remaining / denomValue;
    unsigned int maxFit = std::min(perfectFit, coin.getCount());
    
    for (unsigned int i = 0; i <= maxFit; ++i)
    {
        unsigned int value = denomValue * i;
        if (value <= remaining) {
            unsigned int newRemaining = remaining - value;
            coinsState[denom] = i;
            if (index > 0) {
                getCoinNthCombination(newRemaining, coinList, coinsState, index - 1, callback);
            }
            else if (newRemaining == 0)
            {
                callback(coinsState);
            }
        }
    }       
}

std::vector<unsigned int> Helper::getBestCoinCombination(unsigned int remaining, const std::vector<Coin>& coinList)
{
    std::vector<unsigned int> bestCoinState;
    bestCoinState.reserve(NUM_DENOMS);
    unsigned int bestNumCoins = static_cast<unsigned int>(-1);
    bool found = false;
    unsigned int defaultState[NUM_DENOMS] = {0};

    getCoinNthCombination(remaining, coinList, defaultState, coinList.size() - 1, 
        [&](unsigned int coinsState[NUM_DENOMS])
        {
            unsigned int numCoins = getCoinsStateSum(coinsState);
            if (numCoins < bestNumCoins)
            {
                bestCoinState = std::vector<unsigned int>(coinsState, coinsState + NUM_DENOMS);
                bestNumCoins = numCoins;
                found = true;
            }
        }
    );

    if (!found)
    {
        throw std::runtime_error("Cannot find coins for change");
    }

    return bestCoinState;
}
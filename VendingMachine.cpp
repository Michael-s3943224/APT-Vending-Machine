#include "VendingMachine.h"

VendingMachine::VendingMachine() {};

void VendingMachine::load(const std::string& stockFile, const std::string& coinFile)
{
    //clear the lists in case we are reloading more
    coinList.clear();
    stockList.clear();

    std::vector<Stock> stockVector;

    //try to load the the stock file and coin file else rethrow the error
    try{
        stockVector = Helper::tryLoadStockFile(stockFile);
        coinList = Helper::tryLoadCoinsFile(coinFile);
    }
    catch (const std::runtime_error& e){
        throw std::runtime_error(e.what());
    }

    //sort the stock vector by descending order of item name
    std::sort(stockVector.begin(), stockVector.end(), [](const Stock & a, const Stock & b){
        return Helper::stringLower(a.getName()) > Helper::stringLower(b.getName());
    });
    //this is so we can prepend to the linked list with O(1) time complexity
    for (size_t i = 0; i < stockVector.size(); i++) {
        stockList.prepend(stockVector[i]);
    }

    //sort the coin list be increasing order of the denomination value
    std::sort(coinList.begin(), coinList.end(), [](const Coin& a, const Coin& b){
        return Helper::denomToValue(a.getDenom()) < Helper::denomToValue(b.getDenom());
    });
}

void VendingMachine::save(const std::string& stockFile, const std::string& coinFile)
{
    //save the coinList into a file
    Helper::saveCoinList(coinFile, coinList);
    //save the stockList into a file
    Helper::saveStockList(stockFile, stockList);
    std::cout << "Stock list and coin list has been saved" << std::endl;
    std::cout << std::endl;
}

void VendingMachine::resetStock()
{
    //loop through each item in stockList and set on hand to the default amount
    stockList.forEach([](Stock& stock){
        stock.setOnHand(DEFAULT_STOCK_LEVEL);
    });
    std::cout << "All stock has been reset to the default level of " << DEFAULT_STOCK_LEVEL << std::endl;
    std::cout << std::endl;
}

void VendingMachine::resetCoin()
{
    //loop through each coin in coinList and set count to the default amount
    for (Coin& coin : coinList) {
        coin.setCoinCount(DEFAULT_COIN_COUNT);
    }
    std::cout << "All coins have been reset to the default level of " << DEFAULT_COIN_COUNT << std::endl;
    std::cout << std::endl;
}

unsigned int VendingMachine::getUserItemIndexPersistent(const std::string& prompt)
{
    //call getUserInputPersistent with the following validating lambda
    return Helper::getUserInputPersistent<unsigned int>(prompt, [this](const std::string& s) {
        std::string itemId = "";

        //check if the input item id is valid
        try{
            itemId = Helper::tryParseItemId(s);
        }
        catch(const std::runtime_error& e) {
            throw std::runtime_error(e.what());
        }

        //check if the input item id exists in the stockList
        unsigned int foundIndex = LinkedList::invalidPos;

        foundIndex = this->stockList.findFirst([&itemId](const Stock& stock){
            return stock.getId() == itemId;
        });

        if (foundIndex == LinkedList::invalidPos)
        {
            throw std::runtime_error("Item Id does not exist");
        }

        return foundIndex;
    });
}

std::string VendingMachine::getUserNamePersistent(const std::string& prompt)
{
    //call getUserInputPersistent with tryParseName validator
    return Helper::getUserInputPersistent<std::string>(prompt, Helper::tryParseName);
}

std::string VendingMachine::getUserDescriptionPersistent(const std::string& prompt)
{
    //call getUserInputPersistent with tryParseDescription validator
    return Helper::getUserInputPersistent<std::string>(prompt, Helper::tryParseDescription);
}

Price VendingMachine::getUserPricePersistent(const std::string& prompt)
{
    //call getUserInputPersistent with tryParsePrice validator
    return Helper::getUserInputPersistent<Price>(prompt, Helper::tryParsePrice);
}

Denomination VendingMachine::getUserDenomPersistent(const std::string& prompt)
{
    //call getUserInputPersistent with the following validating lambda
    return Helper::getUserInputPersistent<Denomination>(prompt, [](const std::string s) {
        Denomination denom = FIVE_CENTS;
        unsigned int denomVal = 0;

        //check if the input denom is an integer
        try{
            denomVal = Helper::tryParseInt(s);
        }
        catch(const std::runtime_error& e){
            throw std::runtime_error("Denomination needs to be a valid integer");
        }

        //check if the input denom integer is an actual denom value
        try{
            denom = Helper::tryParseDenom(denomVal);
        }
        catch(const std::runtime_error& e){
            std::string priceString = Helper::valueToPrice(denomVal).getString();
            throw std::runtime_error(priceString + " is not a valid denomination of money");
        }

        return denom;
    });
}

std::string VendingMachine::generateNextId()
{
    unsigned int availableIds = STOCK_MAX_ID - STOCK_MIN_ID + 1;

    //check if we've already exhausted all possible item ids
    //ASSUMPTION: all item ids in stockList are unique (we did not modify them elsewhere)
    if (stockList.size() >= availableIds)
    {
        throw std::runtime_error("Ran out of Item Id's");
    }

    //get all the item ids as a list of integers
    //ASSUMPTION: all item ids are an integer past the first character (we did not modify them elsewhere)
    std::vector<int> ids = stockList.getTransformedValues<int>([](const Stock& s){
        int numberPartStartIndex = 1;
        return Helper::tryParseInt(s.getId().substr(numberPartStartIndex));
    });

    //sort the item ids so we can find the gap id
    std::sort(ids.begin(), ids.end());

    //find the gap id
    int idEmptyAfter = STOCK_MIN_ID - 1;
    bool foundGap = false;

    for (unsigned int i = 0; i < ids.size() && !foundGap; ++i)
    {
        int newId = ids.at(i);
        if (newId > idEmptyAfter + 1)
        {
            foundGap = true;
        }
        else
        {
            idEmptyAfter = newId;
        }
    }

    int newId = idEmptyAfter + 1;

    //create the id string with the gap id
    //ASSUMPTION: id list does not contain ids with more than 4 digits
    int digits = Helper::digitCount(newId);
    int zeroPadding = IDLEN - 1 - digits;
    char padChar = '0';
    std::string result = STOCK_ID_PREFIX + std::string(zeroPadding, padChar) + std::to_string(newId);

    return result;
}

Stock VendingMachine::getUserItem(const std::string& newItemId)
{
    std::cout << "The id of the new stock will be: " << newItemId << std::endl;

    std::string name = "";
    std::string desc = "";
    Price price;

    //try to get the item name, descrition and price from the user
    try{
        name = getUserNamePersistent("Enter the item name: ");
        desc = getUserDescriptionPersistent("Enter the item description: ");
        price = getUserPricePersistent("Enter the price for the item: ");
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("Terminated add item");
    }

    //if we succeeded, create the stock item
    Stock stock(newItemId, name, desc, price, DEFAULT_STOCK_LEVEL);
    return stock;
}

void VendingMachine::addUserItem()
{
    std::string newItemId = "";
    Stock newItem;
    bool success = true;
    std::string terminatedMsg = "Terminated add item";

    //try to generate the new item id
    try {
        newItemId = generateNextId();
    } catch(const std::runtime_error& e) {
        success = false;
        std::cout << ERROR_PREFIX << e.what() << std::endl;
        std::cout << terminatedMsg << std::endl;
    }
    
    //we were able to generate the item id, now try to prompt the item info from the user
    if (success)
    {
        try {
            newItem = getUserItem(newItemId);
        } catch(const std::runtime_error& e) {
            success = false;
            std::cout << terminatedMsg << std::endl;
        }
    }

    //we were able to get the item info from the user, now add the item to the stockList
    if (success)
    {
        //find the index to insert before so we can keep the ascending order of item names
        //ASSUMPTION: stockList is already sorted (we did not modify it elsewhere)
        //DISCLAIMER: this does two linear searches but I don't care
        unsigned int insertBeforeIndex = stockList.findFirst([&newItem](const Stock& s){
            return Helper::stringLower(newItem.getName()) <= Helper::stringLower(s.getName());
        });

        if (stockList.empty())
        {
            stockList.prepend(newItem);
        }
        else if (insertBeforeIndex == LinkedList::invalidPos)
        {
            stockList.append(newItem);
        }
        else
        {
            stockList.insertBefore(insertBeforeIndex, newItem);
        }

        std::cout << "\"" << newItem.getId() << " - " << newItem.getName() << " - " << newItem.getDescription() <<  "\" has been added to the menu." << std::endl;
    }
    std::cout << std::endl;
}

void VendingMachine::removeUserItem()
{
    bool success = true;
    std::string terminatedMsg = "Terminated Remove Item";

    //make sure the stockList is not empty so we can actually remove an item
    if (stockList.empty())
    {
        success = false;
        std::cout << ERROR_PREFIX << "Cannot remove more items from an empty stock list" << std::endl;
        std::cout << terminatedMsg << std::endl;
    }

    //after making sure the stockList is not empty, try to find the index of the item in stockList
    unsigned int foundItemIndex = LinkedList::invalidPos;
    if (success)
    {
        try{
            foundItemIndex = getUserItemIndexPersistent("Enter the item id of the item to remove from the menu: ");
        } catch(const std::exception& e){
            success = false;
            std::cout << terminatedMsg << std::endl;
        }
    }

    //if we haven't terminated yet, remove the item in the stockList at the found index
    //DISCLAIMER: this does two linear searches but I don't care
    if (success)
    {
        Stock removedStock = stockList.removeAt(foundItemIndex);
        std::cout << "\"" << removedStock.getId() <<  " - " << removedStock.getName() << " - " << removedStock.getDescription() << 
            "\" has been removed from the system." << std::endl;
    }

    std::cout << std::endl;
}

void VendingMachine::displayCoins()
{
    //display the coins with the correct allignment and stuff
    int denomWidth = 16;
    int countWidth = 10;
    char horizontalSep = '-';
    char verticalSep = '|';
    int rowCharLen = denomWidth + countWidth + COIN_ATTRIB - 1;
    std::string title = "Coins Summary";

    std::cout << title << std::endl;
    std::cout << std::string(title.length(), horizontalSep) << std::endl;
    std::cout << std::left << std::setw(denomWidth) << "Denomination" << verticalSep << std::right << std::setw(countWidth) << "Count " << std::endl;
    std::cout << std::string(rowCharLen, horizontalSep) << std::endl;
    for (Coin coin: coinList)
    {
        std::cout << std::left << std::setw(denomWidth) << Helper::denomToString(coin.getDenom()) << verticalSep << std::right << std::setw(countWidth) << coin.getCount() << std::endl;
    }
    std::cout << std::endl;
}

void VendingMachine::displayStock() 
{
    //display the items with the correct allignment and stuff
    int idWidth = 5;
    int nameWidth = 40;
    int availableWidth = 11;
    int priceWidth = 8;
    char horizontalSep = '-';
    char verticalSep = '|';
    int rowCharLen = idWidth + nameWidth + availableWidth + priceWidth + STOCK_ATTRIB - 2;
    std::string title = "Items Menu";

    std::cout << title << std::endl;
    std::cout << std::string(title.length(), horizontalSep) << std::endl;
    std::cout << std::left << std::setw(idWidth) << "ID" << verticalSep << std::left << std::setw(nameWidth) << "Name" << verticalSep << std::left << std::setw(availableWidth) << " Available" << verticalSep << std::left << std::setw(priceWidth) << " Price" << std::endl;
    std::cout << std::string(rowCharLen, horizontalSep) << std::endl;
    stockList.forEach([=, &title](Stock& stock){
        //woah what is this?
        //"=" means to pass in all the local variables into this lambda
        //"&title" is here because I don't want to copy it
        std::cout << std::left << std::setw(idWidth) << stock.getId() << verticalSep << std::left << std::setw(nameWidth) << stock.getName() << verticalSep << std::left << std::setw(availableWidth) << stock.getOnHand() << verticalSep << std::left << std::setw(priceWidth) << stock.price.getString() << std::endl;
    });

    //if the stockList is empty print a center alligned special message
    if (stockList.empty())
    {
        std::string emptyMsg = "EMPTY ITEM LIST ;(";
        std::cout << std::string((rowCharLen - emptyMsg.length()) / 2, ' ') << emptyMsg << std::endl;
    }
    std::cout << std::endl;
}

void VendingMachine::purchaseItem()
{
    //Oh boy this one was hard to implement.

    std::string terminatedMsg = "Terminated Purchase Item";

    //try to get the index of the item in the stock list from the user's entered item id string
    bool interrupted = false;
    unsigned int purchaseItemIndex = LinkedList::invalidPos;
    try {
        purchaseItemIndex = getUserItemIndexPersistent("Please enter the id of the item you wish to purchase: ");
    }
    catch(const std::exception& e) {
        interrupted = true;
        std::cout << terminatedMsg << std::endl;
    }

    //had to do nested if statement because of the way references work, please let me have multiple return statements
    //if we managed to get the index of the item in the stock list
    if (!interrupted)
    {
        //get the reference of the item at the stock list index
        Stock& stockRef = stockList.at(purchaseItemIndex);

        //check if we have the item in stock
        if (stockRef.getOnHand() == 0)
        {
            interrupted = true;
            std::cout << ERROR_PREFIX << "Cannot purchase that item as there is none left" << std::endl;
            std::cout << terminatedMsg << std::endl;
        }

        //a dictionary to store the number of each denomination put into the vending machine
        unsigned int coinsPutIn[NUM_DENOMS] = {0};

        unsigned int moneyTarget = stockRef.getPrice().getValue();
        unsigned int moneyIn = 0;

        //now we checked that the item is in stock, the user is then prompted to put in denomination until they can afford the item
        if (!interrupted)
        {
            std::cout << "You have selected \"" << stockRef.getName() << " - " << stockRef.getShortDescription() << "\". This will cost you " << stockRef.getPrice().getString() << "." << std::endl;
            std::cout << "Please hand over the money - type in the value of each note/coin in cents." << std::endl;
            std::cout << "Please enter or ctrl-d on a new line to cancel this purchase:" << std::endl;

            //keep prompting the user until they can afford the item or they terminate
            while (moneyIn < moneyTarget && !interrupted)
            {
                unsigned int moneyOwe = moneyTarget - moneyIn;
                Price priceOwe = Helper::valueToPrice(moneyOwe);
                Denomination denom = FIVE_CENTS;

                try{
                    denom = getUserDenomPersistent("You still need to give us " + priceOwe.getString() + ": ");
                    //if we manage to get a valid denomination value from the user
                    //increment the number of that denomination put in
                    //and add to the money put in
                    coinsPutIn[denom]  += 1;
                    moneyIn += Helper::denomToValue(denom);
                }
                catch(const std::exception& e){
                    interrupted = true;
                    std::cout << terminatedMsg << std::endl;
                }
            }
        }

        unsigned int change = 0;

        //now that the user has put in enough money and has not terminated, the vending machine needs to give change to the user
        if (!interrupted)
        {
            //this round was in case if we didn't get a price in multiples of 5
            //but I guess it doesn't matter anymore since we force the stock file to have prices divisible by 5
            change = Helper::round(moneyIn - moneyTarget, FIVE_CENTS_VAL);

            //put the coins the user put in, into the system, just in case we need them for change
            for (Coin& coin: coinList)
            {
                Denomination denom = coin.getDenom();
                coin.addCoinCount(coinsPutIn[denom]);
            }

            //if the there is no change, then we don't need to give any coins to the user and the transaction ends
            if (change == 0)
            {
                stockRef.removeOnHand(1);
                std::cout << "Here is your " << stockRef.getName() << " with no change" << std::endl;
                interrupted = true;
            }
        }

        //now we know the user has change, we need to calculate which coins to give to the user
        if (!interrupted)
        {   
            bool foundChange = true;
            std::vector<unsigned int> coinsOut;

            try {
                coinsOut = Helper::getBestCoinCombination(change,  coinList);
            }
            catch(const std::runtime_error& e) {
                foundChange = false;
            }

            //if the change left is still not 0, then we are missing the coins needed for the change
            if (!foundChange)
            {
                //take the coins back out from the system and give it back to the user
                for (Coin& coin: coinList)
                {
                    Denomination denom = coin.getDenom();
                    coin.removeCoinCount(coinsPutIn[denom]);
                }

                std::cout << "We do not have enough coins for the change" << std::endl;
                std::cout << terminatedMsg << std::endl;
            }
            else
            {
                //now we know we can actually give the change

                //decrement the stock onhand
                stockRef.removeOnHand(1);
                std::cout << "Here is your " << stockRef.getName() << " and change of " << Helper::valueToPrice(change).getString() << ": ";

                //loop through the coins giving to the user dictionary and remove the correspeding amount from the coinList
                for (int i = coinList.size() - 1; i >= 0; --i) 
                {
                    Denomination denom = coinList.at(i).getDenom();
                    unsigned int denomAmountOut = coinsOut[denom];
                    coinList.at(i).removeCoinCount(denomAmountOut);

                    //we looped from the back so we can print out change from highest to lowest
                    if (denomAmountOut > 1) {
                        std::cout << denomAmountOut << "X";
                    }
                    
                    if (denomAmountOut > 0) {
                        std::cout << Helper::denomToShortString(denom) << " ";
                    }
                }
                std::cout << std::endl;
            }
        }
    }
    
    std::cout << std::endl;
}

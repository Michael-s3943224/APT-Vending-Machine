#include <iostream>
#include "LinkedList.h"
#include "Helper.h"
#include "VendingMachine.h"

// -=-=-=-=-=-=- PLEASE READ README FILE FOR TESTING PROCESS -=-=-=-=-=-=-=-

/**
 * manages the running of the program, initialises data structures, loads
 * data, display the main menu, and handles the processing of options. 
 * Make sure free memory and close all files before exiting the program.
 **/

// all the menu options
enum MenuOption
{
    MENU_DISPLAY_ITEMS = 1,
    MENU_PURCHASE_ITEM = 2,
    MENU_SAVE_AND_EXIT = 3,
    MENU_ADD_ITEM = 4,
    MENU_REMOVE_ITEM = 5,
    MENU_DISPLAY_COINS = 6,
    MENU_RESET_STOCK = 7,
    MENU_RESET_COINS = 8,
    MENU_ABORT_PROGRAM = 9
};

// just display all the options
void displayMainMenu()
{
    std::cout << "Main Menu:" << std::endl;
    std::cout << "  1.Display Items" << std::endl;
    std::cout << "  2.Purchase Items" << std::endl;
    std::cout << "  3.Save and Exit" << std::endl;
    std::cout << "Administrator-Only Menu:" << std::endl;
    std::cout << "  4.Add Item" << std::endl;
    std::cout << "  5.Remove Item" << std::endl;
    std::cout << "  6.Display Coins" << std::endl;
    std::cout << "  7.Reset Stock" << std::endl;
    std::cout << "  8.Reset Coins" << std::endl;
    std::cout << "  9.Abort Program" << std::endl;
}

unsigned int getUserChoicePersistent(const std::string& prompt)
{
    return Helper::getUserInputPersistent<unsigned int>(prompt, [](const std::string& s){
        unsigned int result = MENU_DISPLAY_ITEMS;

        // check if the user choice is an integer
        try{
            result = Helper::tryParseInt(s);
        }
        catch(const std::runtime_error& e){
            throw std::runtime_error("input was not a valid number");
        }

        //then check if the user choice is in the range of the menu options
        if (result < MENU_DISPLAY_ITEMS || result > MENU_ABORT_PROGRAM)
        {
            throw std::runtime_error("menu item selected is not valid");
        }

        return result;
    });
}

// just have everything in a seperate function
// so I can use throw exception to break out of it and prevent nested if statements
// because you guys won't let me use multiple return statements
void start(int argc, char **argv)
{
    int numArgs = 3;

    // check if we have the correct number of command line arguments
    if (argc != numArgs)
    {
        throw std::runtime_error("Program Exited: Invalid number of command line arguments, only 3 arguments allowed.");
    }

    std::string stockFileName = argv[1];
    std::string coinFileName = argv[2];

    VendingMachine vendingMachine;

    // try to load the stock file and coin
    try{
        vendingMachine.load(stockFileName, coinFileName);
    }
    catch(const std::exception& e) {
        throw std::runtime_error(ERROR_PREFIX + std::string(e.what()));
    }

    bool exit = false;

    // the main loop keeping going if we haven't exited or reach EOF
    while (!exit && !std::cin.eof())
    {  
        // display the main menu
        displayMainMenu();

        // prompt the user for a menu option
        unsigned int userChoice = MENU_DISPLAY_ITEMS;
        try {
            userChoice = getUserChoicePersistent("Select your option (1-9): ");
        } catch(const std::runtime_error& e) {
            // the user terminated so we exit
            exit = true;
        }

        // if we have not terminated, excute one of the commands
        if (!exit)
        {
            std::cout << std::endl;
            if (userChoice == MENU_DISPLAY_ITEMS) {
                vendingMachine.displayStock();
            }
            else if (userChoice == MENU_PURCHASE_ITEM) {
                vendingMachine.purchaseItem();
            }
            else if (userChoice == MENU_SAVE_AND_EXIT) {
                vendingMachine.save(stockFileName, coinFileName);
                exit = true;
            }
            else if (userChoice == MENU_ADD_ITEM) {
                vendingMachine.addUserItem();
            }
            else if (userChoice == MENU_REMOVE_ITEM) {
                vendingMachine.removeUserItem();
            }
            else if (userChoice == MENU_DISPLAY_COINS) {
                vendingMachine.displayCoins();
            }
            else if (userChoice == MENU_RESET_STOCK) {
                vendingMachine.resetStock();
            }
            else if (userChoice == MENU_RESET_COINS) {
                vendingMachine.resetCoin();
            }
            else if (userChoice == MENU_ABORT_PROGRAM) {
                std::cout <<"Program Terminated" << std::endl;
                exit = true;
            }
        }
    }
}

// main
int main(int argc, char **argv)
{
    try{
        start(argc, argv);
    }
    catch(const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }
    
    return EXIT_SUCCESS;
}

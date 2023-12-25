#include "MerkelMain.h"
#include <iostream>
#include <vector>
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include "Candlestick.h"
#include "CandlestickGraph.h"
#include "VolumeGraph.h"

MerkelMain::MerkelMain()
{

}

void MerkelMain::init()
{
    int input;
    currentTime = orderBook.getEarliestTime();

    wallet.insertCurrency("BTC", 10);

    while(true)
    {
        printMenu();
        input = getUserOption();
        processUserOption(input);
    }
}


void MerkelMain::printMenu()
{
    // 1 print help
    std::cout << "1: Print help " << std::endl;
    // 2 print exchange stats
    std::cout << "2: Print exchange stats" << std::endl;
    // 3 make an offer
    std::cout << "3: Make an offer " << std::endl;
    // 4 make a bid 
    std::cout << "4: Make a bid " << std::endl;
    // 5 print wallet
    std::cout << "5: Print wallet " << std::endl;
    // 6 continue   
    std::cout << "6: Continue " << std::endl;
    // 7 print candlesticks   
    std::cout << "7: Print Candlesticks " << std::endl;
    // 8 print volume graph   
    std::cout << "8: Print Volume Graph " << std::endl;

    std::cout << "============== " << std::endl;

    std::cout << "Current time is: " << currentTime << std::endl;
}

void MerkelMain::printHelp()
{
    std::cout << "Help - your aim is to make money. Analyse the market and make bids and offers. " << std::endl;
}

void MerkelMain::printMarketStats()
{
    for (std::string const& p : orderBook.getKnownProducts())
    {
        std::cout << "Product: " << p << std::endl;
        std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask, 
                                                                p, currentTime);
        std::cout << "Asks seen: " << entries.size() << std::endl;
        std::cout << "Max ask: " << OrderBook::getHighPrice(entries) << std::endl;
        std::cout << "Min ask: " << OrderBook::getLowPrice(entries) << std::endl;



    }
    // std::cout << "OrderBook contains :  " << orders.size() << " entries" << std::endl;
    // unsigned int bids = 0;
    // unsigned int asks = 0;
    // for (OrderBookEntry& e : orders)
    // {
    //     if (e.orderType == OrderBookType::ask)
    //     {
    //         asks ++;
    //     }
    //     if (e.orderType == OrderBookType::bid)
    //     {
    //         bids ++;
    //     }  
    // }    
    // std::cout << "OrderBook asks:  " << asks << " bids:" << bids << std::endl;

}

void MerkelMain::enterAsk()
{
    std::cout << "Make an ask - enter the amount: product,price, amount, eg  ETH/BTC,200,0.5" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::enterAsk Bad input! " << input << std::endl;
    }
    else {
        try {
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2], 
                currentTime, 
                tokens[0], 
                OrderBookType::ask 
            );
            obe.username = "simuser";
            if (wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(obe);
            }
            else {
                std::cout << "Wallet has insufficient funds . " << std::endl;
            }
        }catch (const std::exception& e)
        {
            std::cout << " MerkelMain::enterAsk Bad input " << std::endl;
        }   
    }
}

void MerkelMain::enterBid()
{
    std::cout << "Make an bid - enter the amount: product,price, amount, eg  ETH/BTC,200,0.5" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::enterBid Bad input! " << input << std::endl;
    }
    else {
        try {
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2], 
                currentTime, 
                tokens[0], 
                OrderBookType::bid 
            );
            obe.username = "simuser";

            if (wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(obe);
            }
            else {
                std::cout << "Wallet has insufficient funds . " << std::endl;
            }
        }catch (const std::exception& e)
        {
            std::cout << " MerkelMain::enterBid Bad input " << std::endl;
        }   
    }
}

void MerkelMain::printWallet()
{
    std::cout << wallet.toString() << std::endl;
}
        
void MerkelMain::gotoNextTimeframe()
{
    std::cout << "Going to next time frame. " << std::endl;
    for (std::string p : orderBook.getKnownProducts())
    {
        std::cout << "matching " << p << std::endl;
        std::vector<OrderBookEntry> sales =  orderBook.matchAsksToBids(p, currentTime);
        std::cout << "Sales: " << sales.size() << std::endl;
        for (OrderBookEntry& sale : sales)
        {
            std::cout << "Sale price: " << sale.price << " amount " << sale.amount << std::endl; 
            if (sale.username == "simuser")
            {
                // update the wallet
                wallet.processSale(sale);
            }
        }
        
    }

    currentTime = orderBook.getNextTime(currentTime);
}
 
int MerkelMain::getUserOption()
{
    int userOption = 0;
    std::string line;
    std::cout << "Type in 1-8" << std::endl;
    std::getline(std::cin, line);
    try{
        userOption = std::stoi(line);
    }catch(const std::exception& e)
    {
        // 
    }
    std::cout << "You chose: " << userOption << std::endl;
    return userOption;
}

void MerkelMain::printCandlesticks()
{
    // ask for order type
    OrderBookType type;
    std::cout << "Enter order type (ask/bid)" << std::endl;
    std::string typeInput;
    std::getline(std::cin, typeInput);  

    if (typeInput != "ask" && typeInput != "bid") {
        std::cout << "Invalid type. Choose ask or bid." << std::endl;
        return;
    } else {
        type = OrderBookEntry::stringToOrderBookType(typeInput);
    }

    // ask for product
    std::string product;
    std::vector<std::string> knownProducts = orderBook.getKnownProducts();
    std::cout << "Choose product." << std::endl;
    for (int i = 1; i < knownProducts.size() + 1; i++)
    {
        std::cout << i << ": " << knownProducts[i - 1] << std::endl;
    }

    std::string productInput;
    std::getline(std::cin, productInput);
    try {
        product = knownProducts[stoi(productInput) - 1];
        if (product == "") {
            throw std::exception();
        }
    } catch (const std::exception& e) {
        std::cout << "Invalid product." << std::endl;
        return;
    }

    // ask for time period
    std::cout << "Do you want a custom time period? Else, default is 15 minutes. (yes/no)" << std::endl;
    std::string wantPeriod;
    std::getline(std::cin, wantPeriod);  

    int period = 180;
    if (wantPeriod == "yes") {
        std::cout << "Enter time period in minutes" << std::endl;
        std::string periodInput;
        std::getline(std::cin, periodInput);
        try {
            period = stoi(periodInput) * 60 / 5;
            if (period < 1) {
                throw std::exception();
            }
        } catch (const std::exception& e) {
            std::cout << "Invalid input. Defaulting to 15 minutes." << std::endl;
        }
    } else if (wantPeriod != "no") {
        std::cout << "Invalid input. Defaulting to 15 minutes." << std::endl;
    }

    // ask if they want to show the first candlestick
    std::cout << "Do you want to show the first period candlestick? Open value will be first price registered (yes/no)" << std::endl;
    std::string showFirstInput;
    std::getline(std::cin, showFirstInput);

    bool showFirst = false;
    if (showFirstInput == "yes") {
        showFirst = true;
    } else if (showFirstInput != "no") {
        std::cout << "Invalid input. Defaulting to no." << std::endl;
    }

    // get and plot candlesticks
    CandlestickGraph candlesticks{orderBook, type, product, period, showFirst};
    candlesticks.plot();
}

void MerkelMain::printVolumeGraph() {
    // ask for product
    std::string product;
    std::vector<std::string> knownProducts = orderBook.getKnownProducts();
    std::cout << "Choose product." << std::endl;
    for (int i = 1; i < knownProducts.size() + 1; i++)
    {
        std::cout << i << ": " << knownProducts[i - 1] << std::endl;
    }

    std::string productInput;
    std::getline(std::cin, productInput);
    try {
        product = knownProducts[stoi(productInput) - 1];
        if (product == "") {
            throw std::exception();
        }
    } catch (const std::exception& e) {
        std::cout << "Invalid product." << std::endl;
        return;
    }

    // ask for time period
    std::cout << "Do you want a custom time period? Else, default is 15 minutes. (yes/no)" << std::endl;
    std::string wantPeriod;
    std::getline(std::cin, wantPeriod);  

    int period = 180;
    if (wantPeriod == "yes") {
        std::cout << "Enter time period in minutes" << std::endl;
        std::string periodInput;
        std::getline(std::cin, periodInput);
        try {
            period = stoi(periodInput) * 60 / 5;
            if (period < 1) {
                throw std::exception();
            }
        } catch (const std::exception& e) {
            std::cout << "Invalid input. Defaulting to 15 minutes." << std::endl;
        }
    } else if (wantPeriod != "no") {
        std::cout << "Invalid input. Defaulting to 15 minutes." << std::endl;
    }

    VolumeGraph{orderBook, product, period}.plot();
}

void MerkelMain::processUserOption(int userOption)
{
    if (userOption == 0) // bad input
    {
        std::cout << "Invalid choice. Choose 1-8" << std::endl;
    }
    if (userOption == 1) 
    {
        printHelp();
    }
    if (userOption == 2) 
    {
        printMarketStats();
    }
    if (userOption == 3) 
    {
        enterAsk();
    }
    if (userOption == 4) 
    {
        enterBid();
    }
    if (userOption == 5) 
    {
        printWallet();
    }
    if (userOption == 6) 
    {
        gotoNextTimeframe();
    }
    if (userOption == 7) 
    {
        printCandlesticks();
    } 
    if (userOption == 8) 
    {
        printVolumeGraph();
    }       
}

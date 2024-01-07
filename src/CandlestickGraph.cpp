#include "CandlestickGraph.h"
#include "TerminalWindow.h"
#include <iostream>
#include <iomanip>
#include <set>

// terminal output colors
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"

CandlestickGraph::CandlestickGraph(OrderBook &orderBook, OrderBookType &_type, std::string &_product, int &_period, bool &showFirst) // CandlestickGraph constructor
{
    type = _type;
    product = _product;
    period = _period;
    candlesticks = getCandlesticks(orderBook, showFirst);
}

std::vector<Candlestick> CandlestickGraph::getCandlesticks(OrderBook &orderBook, bool &showFirst) 
{
    std::vector<Candlestick> candlesticks;

    // get earliest time as current time
    std::string currentTime = orderBook.getEarliestTime();

    // get order with the specified type and product
    std::vector<OrderBookEntry> orders = orderBook.getOrders(type, product);

    // set first open value as first (earliest) order price
    double open = orders[0].price;

    // set next time counter to 0. This counter checks if the order is in our current time period
    int nextTimeCounter = 0;

    // set current period orders to empty vector
    std::vector<OrderBookEntry> currentPeriodOrders;

    // iterate through orders
    for (const OrderBookEntry &order : orders) {
        if (order.timestamp == currentTime) { // if order is in current time period, add it to current period orders
            currentPeriodOrders.push_back(order);
        } else if (nextTimeCounter <= period) { // if order is still in current time period, add it to current period orders and increment next time counter
            currentPeriodOrders.push_back(order);

            currentTime = order.timestamp;
            nextTimeCounter += 1;
        } else { // if order is not in current time period, create candlestick with current period orders and open value
            Candlestick candlestick{currentPeriodOrders, open};
            candlesticks.push_back(candlestick);

            // set next open value as last close value
            open = candlestick.close; 

            // reset current period values and add current order to our new period orders
            currentPeriodOrders.clear();
            currentPeriodOrders.push_back(order);
            currentTime = order.timestamp;
            nextTimeCounter = 0;
        }
    }

    // create candlestick with the orders that are left, if there are any
    if (currentPeriodOrders.size() > 0) {
        Candlestick candlestick{currentPeriodOrders, open};
        candlesticks.push_back(candlestick);
    }

    // remove first candlestick if the user does not want to show it
    if (!showFirst) {
        candlesticks.erase(candlesticks.begin());
    }

    return candlesticks;
}

void CandlestickGraph::plot() 
{
    // get graph width and console width
    int graphWidth = candlesticks.size() * 11 + 14;
    int consoleWidth = TerminalWindow::getConsoleWidth();

    // check if graph can be displayed in console, else display error message
    if (graphWidth > consoleWidth) {
        TerminalWindow::error(
            "Can't display graph in this console, try a wider console with at least " + std::to_string(graphWidth) + ". Current width " + std::to_string(consoleWidth)
        );
        return;
    }

    // print graph header
    for (int i = 0; i < graphWidth; i++) {
        std::cout << "═";
    }
    std::cout << std::endl;

    std::string typeString = (type == OrderBookType::ask) ? "ask" : "bid";
    std::cout << typeString << " | " << product << " | " << period * 5 / 60 << " minutes" << std::endl;
    
    for (int i = 0; i < graphWidth; i++) {
        std::cout << "═";
    }
    std::cout << std::endl;

    // get Y axis values
    std::vector<double> orderedValues = getOrderedValues();

    // iterate through Y values
    for (const double &Yvalue : orderedValues) {
        // print Y value
        std::cout << std::setw(12) << Yvalue << " │";

        // iterate through candlesticks to check if they are in the current Y value
        for (const Candlestick &candlestick : candlesticks) {
            // set candlestick color
            std::string color = (candlestick.open > candlestick.close) ? RED : GREEN;
            
            // print candlestick character for current Y value. Using desired color and reseting color after printing
            std::cout << color;
            if ((candlestick.open >= Yvalue && candlestick.close <= Yvalue) || (candlestick.open <= Yvalue && candlestick.close >= Yvalue)) {
                std::cout << " █████████ ";
            } else if (candlestick.high >= Yvalue && candlestick.low <= Yvalue) {
                std::cout << "     │     ";
            } else {
                std::cout << "           ";
            }
            std::cout << RESET;
        }
        std::cout << std::endl;

    }

    // print graph bottom
    for (int i = 0; i < graphWidth; i++) {
        std::cout << "─";
    }
    std::cout << std::endl;

    // print X axis values with timeframes
    std::vector<std::string> timeframes = getTimeframes();
    std::cout <<  "             | ";
    for (const std::string &timeframe : timeframes) {
        std::cout << timeframe << "   ";
    }
    std::cout << std::endl;

}

std::vector<double> CandlestickGraph::getOrderedValues() 
{
    std::vector<double> values;

    // get every candlestick values
    for (const Candlestick &candlestick : candlesticks) {
        values.push_back(candlestick.open);
        values.push_back(candlestick.close);
        values.push_back(candlestick.high);
        values.push_back(candlestick.low);
    }

    // use set to automatically remove duplicates and asign values to that set
    std::set<double> uniqueValues(values.begin(), values.end());
    values.assign(uniqueValues.begin(), uniqueValues.end());

    // sort values in descending order
    std::sort(values.begin(), values.end(), std::greater<double>());

    return values;
}

std::vector<std::string> CandlestickGraph::getTimeframes() 
{
    std::vector<std::string> timeframes;

    // get every candlestick timeframe
    for (const Candlestick &candlestick : candlesticks) {
        std::string time = candlestick.date.substr(11, 8);
        timeframes.push_back(time);
    }

    return timeframes;
}
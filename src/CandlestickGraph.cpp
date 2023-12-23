#include "CandlestickGraph.h"
#include <iostream>
#include <iomanip>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"

CandlestickGraph::CandlestickGraph(OrderBook orderBook, OrderBookType _type, std::string _product, int _period) 
{
    type = _type;
    product = _product;
    period = _period;
    candlesticks = getCandlesticks(orderBook);
}

std::vector<Candlestick> CandlestickGraph::getCandlesticks(OrderBook orderBook) 
{
    std::cout << "processCandlestickGraph " << product << " " << period << std::endl;
    std::cout << "Loading";
    int loadingCounter = 0;
    std::vector<Candlestick> candlesticks;

    std::string earliestTime = orderBook.getEarliestTime();
    std::string currentTime = orderBook.getEarliestTime();
    double open = 0;

    bool isFirst = true;

    while (isFirst || currentTime != earliestTime) {
        isFirst = false;
        std::vector<OrderBookEntry> orders = orderBook.getOrders(type, product, currentTime);
        currentTime = orderBook.getNextTime(currentTime);
        int nextTimeCounter = 0;

        while (nextTimeCounter <= period && currentTime != earliestTime) {
            CandlestickGraph::loadingAnimation(loadingCounter);
            std::vector<OrderBookEntry> newOrders = orderBook.getOrders(type, product, currentTime);

            orders.insert(orders.end(), newOrders.begin(), newOrders.end());

            currentTime = orderBook.getNextTime(currentTime);
            nextTimeCounter += 1;
        }

        Candlestick candlestick = Candlestick::processCandlestick(orders, open);
        open = candlestick.close;
        candlesticks.push_back(candlestick);
        
        CandlestickGraph::loadingAnimation(loadingCounter);
    }
    std::cout << "\r" << "          " << std::endl;

    return candlesticks;
}

void CandlestickGraph::plot() 
{
    int graphWidth = candlesticks.size() * 11 + 12;

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

    std::vector<double> orderedValues = getOrderedValues();
    for (double Xvalue : orderedValues) {
        std::cout << std::setw(10) << Xvalue << " │";

        for (Candlestick candlestick : candlesticks) {
            std::string color = (candlestick.open > candlestick.close) ? RED : GREEN;
            
            std::cout << color;
            if ((candlestick.open >= Xvalue && candlestick.close <= Xvalue) || (candlestick.open <= Xvalue && candlestick.close >= Xvalue)) {
                std::cout << " █████████ ";
            } else if (candlestick.high >= Xvalue && candlestick.low <= Xvalue) {
                std::cout << "     │     ";
            } else {
                std::cout << "           ";
            }
            std::cout << RESET;
        }
        std::cout << std::endl;

    }

    for (int i = 0; i < graphWidth; i++) {
        std::cout << "─";
    }
    std::cout << std::endl;


    std::vector<std::string> timeframes = getTimeframes();
    std::cout <<  "           | ";
    for (std::string timeframe : timeframes) {
        std::cout << timeframe << "   ";
    }
    std::cout << std::endl;

}

std::vector<double> CandlestickGraph::getOrderedValues() 
{
    std::vector<double> values;

    for (Candlestick const& candlestick : candlesticks) {
        values.push_back(candlestick.open);
        values.push_back(candlestick.close);
        values.push_back(candlestick.high);
        values.push_back(candlestick.low);
    }

    std::sort(values.begin(), values.end(), std::greater<double>());
    return values;
}

std::vector<std::string> CandlestickGraph::getTimeframes() 
{
    std::vector<std::string> timeframes;

    for (Candlestick const& candlestick : candlesticks) {
        std::string date = candlestick.date.substr(11, 8);
        timeframes.push_back(date);
    }

    return timeframes;
}

void CandlestickGraph::loadingAnimation(int &loadingCounter) {
    std::cout << "\r";
    if (loadingCounter == 0) {
        std::cout << "Loading   ";
        loadingCounter += 1;
    } else if (loadingCounter == 1) {
        std::cout << "Loading.  ";
        loadingCounter += 1;
    } else if (loadingCounter == 2) {
        std::cout << "Loading.. ";
        loadingCounter += 1;
    } else {
        std::cout << "Loading...";
        loadingCounter = 0;
    }
    std::cout.flush();
}
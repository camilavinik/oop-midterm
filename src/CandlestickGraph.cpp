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
    candlesticks = processCandlestickGraph(orderBook);
}

std::vector<Candlestick> CandlestickGraph::processCandlestickGraph(OrderBook orderBook) 
{
    std::cout << "processCandlestickGraph " << product << " " << period << std::endl;
    std::cout << "Loading ";
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
            std::cout << "\u00B7";
            std::cout.flush();
            std::vector<OrderBookEntry> newOrders = orderBook.getOrders(type, product, currentTime);

            orders.insert(orders.end(), newOrders.begin(), newOrders.end());

            currentTime = orderBook.getNextTime(currentTime);
            nextTimeCounter += 1;
        }

        Candlestick candlestick = Candlestick::processCandlestick(orders, open);
        open = candlestick.close;
        candlesticks.push_back(candlestick);
        std::cout << "\u00B7";
        std::cout.flush();
    }
    std::cout << std::endl;

    return candlesticks;
}

void CandlestickGraph::plotCandlesticks() 
{
    int graphWidth = candlesticks.size() * 11 + 12;

    for (int i = 0; i < graphWidth; i++) {
        std::cout << "-";
    }
    std::cout << std::endl;
    std::string typeString = (type == OrderBookType::ask) ? "ask" : "bid";
    std::cout << typeString << " | " << product << " | " << period * 5 / 60 << " minutes" << std::endl;
    for (int i = 0; i < graphWidth; i++) {
        std::cout << "-";
    }
    std::cout << std::endl;

    std::vector<double> orderedValues = getOrderedValues();
    for (double Xvalue : orderedValues) {
        std::cout << std::setw(10) << Xvalue << " |";

        for (Candlestick candlestick : candlesticks) {
            std::string color = (candlestick.open > candlestick.close) ? RED : GREEN;
            
            std::cout << color;
            if ((candlestick.open >= Xvalue && candlestick.close <= Xvalue) || (candlestick.open <= Xvalue && candlestick.close >= Xvalue)) {
                std::cout << " ========= ";
            } else if (candlestick.high >= Xvalue && candlestick.low <= Xvalue) {
                std::cout << "     |     ";
            } else {
                std::cout << "           ";
            }
            std::cout << RESET;
        }
        std::cout << std::endl;

    }

    for (int i = 0; i < graphWidth; i++) {
        std::cout << "-";
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
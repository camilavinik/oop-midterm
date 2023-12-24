#include "CandlestickGraph.h"
#include <iostream>
#include <iomanip>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"

CandlestickGraph::CandlestickGraph(OrderBook orderBook, OrderBookType _type, std::string _product, int _period, bool showFirst) 
{
    type = _type;
    product = _product;
    period = _period;
    candlesticks = getCandlesticks(orderBook, showFirst);
}

std::vector<Candlestick> CandlestickGraph::getCandlesticks(OrderBook orderBook, bool showFirst) 
{
    std::cout << "processCandlestickGraph " << product << " " << period << std::endl;

    std::vector<Candlestick> candlesticks;

    std::string currentTime = orderBook.getEarliestTime();

    std::vector<OrderBookEntry> orders = orderBook.getOrders(type, product);
    double open = orders[0].price;
    int nextTimeCounter = 0;
    std::vector<OrderBookEntry> currentPeriodOrders;

    for (OrderBookEntry &order : orders) {
        if (order.timestamp == currentTime) {
            currentPeriodOrders.push_back(order);
        } else if (nextTimeCounter <= period) {
            currentPeriodOrders.push_back(order);

            currentTime = order.timestamp;
            nextTimeCounter += 1;
        } else {
            Candlestick candlestick = Candlestick::processCandlestick(currentPeriodOrders, open);
            open = candlestick.close;
            candlesticks.push_back(candlestick);

            currentPeriodOrders.clear();
            currentPeriodOrders.push_back(order);
            currentTime = order.timestamp;
            nextTimeCounter = 0;
        }
    }

    if (currentPeriodOrders.size() > 0) {
        Candlestick candlestick = Candlestick::processCandlestick(currentPeriodOrders, open);
        candlesticks.push_back(candlestick);
    }

    if (!showFirst) {
        candlesticks.erase(candlesticks.begin());
    }

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
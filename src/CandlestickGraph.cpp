#include "CandlestickGraph.h"
#include <iostream>

CandlestickGraph::CandlestickGraph(OrderBook orderBook) 
{
    candlesticks = processCandlestickGraph(orderBook);
}

std::vector<Candlestick> CandlestickGraph::processCandlestickGraph(OrderBook orderBook) 
{
    std::vector<Candlestick> candlesticks;
    int period = 180; // 180 nextTime = 15 minutes

    std::string earliestTime = orderBook.getEarliestTime();
    std::string currentTime = orderBook.getEarliestTime();
    double open = 0;

    bool isFirst = true;

    while (isFirst || currentTime != earliestTime) {
        isFirst = false;
        std::vector<OrderBookEntry> orders = orderBook.getOrders(OrderBookType::ask, "BTC/USDT", currentTime);
        currentTime = orderBook.getNextTime(currentTime);
        int nextTimeCounter = 0;

        while (nextTimeCounter <= period && currentTime != earliestTime) {
            std::vector<OrderBookEntry> newOrders = orderBook.getOrders(OrderBookType::ask, "BTC/USDT", currentTime);

            orders.insert(orders.end(), newOrders.begin(), newOrders.end());

            currentTime = orderBook.getNextTime(currentTime);
            nextTimeCounter += 1;
        }

        Candlestick candlestick = Candlestick::processCandlestick(orders, open);
        open = candlestick.close;
        candlesticks.push_back(candlestick);
    }

    return candlesticks;
}

double CandlestickGraph::getHighValue() 
{
    double high = candlesticks[0].high;

    for (Candlestick const& candlestick : candlesticks) {
        if (candlestick.high > high) {
            high = candlestick.high;
        }
    }

    return high;
}

double CandlestickGraph::getLowValue() 
{
    double low = candlesticks[0].low;

    for (Candlestick const& candlestick : candlesticks) {
        if (candlestick.low < low) {
            low = candlestick.low;
        }
    }

    return low;
}
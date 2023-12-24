#pragma once
#include <vector>
#include "Candlestick.h"
#include "OrderBook.h"

class CandlestickGraph
{
    public:
        std::vector<Candlestick> candlesticks;
        CandlestickGraph(OrderBook orderBook, OrderBookType _type, std::string _product, int _period, bool showFirst);
        void plot();

    private:
        OrderBookType type;
        std::string product;
        int period;
        std::vector<Candlestick> getCandlesticks(OrderBook orderBook, bool showFirst);
        std::vector<double> getOrderedValues();
        std::vector<std::string> getTimeframes();
};

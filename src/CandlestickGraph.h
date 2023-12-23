#pragma once
#include <vector>
#include "Candlestick.h"
#include "OrderBook.h"

class CandlestickGraph
{
    public:
        std::vector<Candlestick> candlesticks;
        CandlestickGraph(OrderBook orderBook);
        void plotCandlesticks();

    private:
        std::vector<Candlestick> processCandlestickGraph(OrderBook orderBook);
        std::vector<double> getOrderedValues();
        std::vector<std::string> getTimeframes();
};

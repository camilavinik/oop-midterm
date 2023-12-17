#pragma once
#include <vector>
#include "Candlestick.h"
#include "OrderBook.h"

class CandlestickGraph
{
    public:
        std::vector<Candlestick> candlesticks;
        CandlestickGraph(OrderBook orderBook);

    private:
        std::vector<Candlestick> processCandlestickGraph(OrderBook orderBook);
        double getHighValue();
        double getLowValue();
};

#pragma once
#include <vector>
#include "Candlestick.h"
#include "OrderBook.h"

class CandlestickGraph
{
    public:
        std::vector<Candlestick> candlesticks;
        CandlestickGraph(OrderBook &orderBook, OrderBookType &_type, std::string &_product, int &_period, bool &showFirst);
        /** Plot Candlestick Graph on console. */
        void plot();

    private:
        OrderBookType type;
        std::string product;
        int period;
        /** Get Candlesticks out of order book, use showFirst to determine if the first candlestick should be created.
         * Returns vector of candlesticks.
        */
        std::vector<Candlestick> getCandlesticks(OrderBook &orderBook, bool &showFirst);
        /** Get candlesticks values in descending order. */
        std::vector<double> getOrderedValues();
        /** Get candlesticks timeframes in ascending order. */
        std::vector<std::string> getTimeframes();
};

#pragma once
#include <string>
#include <vector>
#include "OrderBookEntry.h"

class Candlestick
{
    public:
        std::string date;
        double open;
        double high;
        double low;
        double close;

        Candlestick( std::string date,
                     double open,
                     double close,
                     double high,
                     double low);
                     
        static Candlestick processCandlestick(std::vector<OrderBookEntry>& entries, double open);
};

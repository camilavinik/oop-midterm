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

        Candlestick(std::vector<OrderBookEntry> &entries, double &open);   
};

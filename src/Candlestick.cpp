#include "Candlestick.h"

Candlestick::Candlestick( std::string _date,
                          double _open,
                          double _close,
                          double _high,
                          double _low) 
: date(_date),
  open(_open),
  close(_close),
  high(_high),
  low(_low)
{

}

Candlestick Candlestick::processCandlestick(std::vector<OrderBookEntry>& entries, double open)
{
    double high = entries[0].price;
    double low = entries[0].price;
    double totalAmount = 0;
    double totalValue = 0;

    for (OrderBookEntry const& entry : entries) {
        totalAmount += entry.amount;
        totalValue += entry.price;

        if (entry.price > high) {
            high = entry.price;
        }

        if (entry.price < low) {
            low = entry.price;
        }
    }

    double close = totalValue / totalAmount;

    Candlestick candlestick{entries[0].timestamp, open, close, high, low};

    return candlestick;
}
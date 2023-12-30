#include "Candlestick.h"

Candlestick::Candlestick(std::vector<OrderBookEntry>& entries, double& openValue)
{
    double highValue = entries[0].price;
    double lowValue = entries[0].price;
    double totalAmount = 0;
    double totalValue = 0;

    for (OrderBookEntry const& entry : entries) {
        totalAmount += entry.amount;
        totalValue += entry.price * entry.amount;

        if (entry.price > highValue) {
            highValue = entry.price;
        }

        if (entry.price < lowValue) {
            lowValue = entry.price;
        }
    }

    double closeValue = totalValue / totalAmount;

    date = entries[0].timestamp;
    open = openValue;
    high = highValue;
    low = lowValue;
    close = closeValue;
}
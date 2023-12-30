#include "Candlestick.h"

Candlestick::Candlestick(std::vector<OrderBookEntry>& entries, double& openValue) // Candlestick constructor
{
    // set initial high and low values and total amount and value
    double highValue = entries[0].price;
    double lowValue = entries[0].price;
    double totalAmount = 0;
    double totalValue = 0;

    // iterate through the entries to find high, low, total amount and total value
    for (OrderBookEntry const& entry : entries) {
        totalAmount += entry.amount;
        totalValue += entry.price * entry.amount;

        // set high value if entry price is higher than current high value
        if (entry.price > highValue) {
            highValue = entry.price;
        }

        // set low value if entry price is lower than current low value
        if (entry.price < lowValue) {
            lowValue = entry.price;
        }
    }

    // set candlestick values
    date = entries[0].timestamp;
    open = openValue;
    high = highValue;
    low = lowValue;
    close = totalValue / totalAmount;
}
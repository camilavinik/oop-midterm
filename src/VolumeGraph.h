#pragma once
#include <string>
#include <vector>
#include "VolumeEntry.h"
#include "OrderBook.h"

class VolumeGraph {
    public:
        VolumeGraph(OrderBook orderBook, std::string product, int period);
        /** Plot Volume Graph on console. */
        void plot();
    
    private:
        std::string product;
        int period;
        std::vector<VolumeEntry> volumes;

        /** Get volume entries out of order book.
         * Returns vector of volume entries.
        */
        std::vector<VolumeEntry> getVolumes(OrderBook orderBook);
        /** Get volume entries values in descending order. */
        std::vector<double> getOrderedValues();
        /** Get volume entries start times. */
        std::vector<std::string> getStartTimes();
        /** Get volume entries end times. */
        std::vector<std::string> getEndTimes();
};
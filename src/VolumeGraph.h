#pragma once
#include <string>
#include <vector>
#include "VolumeEntry.h"
#include "OrderBook.h"

class VolumeGraph {
    public:
        VolumeGraph(OrderBook orderBook, std::string product, int period);
        void plot();
    
    private:
        std::string product;
        int period;
        std::vector<VolumeEntry> volumes;
        std::vector<VolumeEntry> getVolumes(OrderBook orderBook);
        std::vector<double> getOrderedValues();
        std::vector<std::string> getStartTimes();
        std::vector<std::string> getEndTimes();
};
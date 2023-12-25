#pragma once
#include <string>

class VolumeEntry {
    public:
        std::string startTime;
        std::string endTime;
        double ask;
        double bid;
        VolumeEntry(std::string startTime, std::string endTime, double ask, double bid);
};
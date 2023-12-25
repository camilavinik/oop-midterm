#pragma once
#include <string>

class VolumeEntry {
    public:
        std::string timestamp;
        double ask;
        double bid;
        VolumeEntry(std::string _timestamp, double ask, double bid);
};
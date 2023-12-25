#include "VolumeEntry.h"

VolumeEntry::VolumeEntry(std::string _startTime, std::string _endTime, double _ask, double _bid) {
    startTime = _startTime;
    endTime = _endTime;
    ask = _ask;
    bid = _bid;
}
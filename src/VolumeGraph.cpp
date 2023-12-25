#include "VolumeGraph.h"
#include <iostream>
#include <iomanip>

#define RESET   "\033[0m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"

VolumeGraph::VolumeGraph(OrderBook orderBook, std::string _product, int _period) {
    product = _product;
    period = _period;
    volumes = getVolumes(orderBook);
}

std::vector<VolumeEntry> VolumeGraph::getVolumes(OrderBook orderBook) {
    std::vector<VolumeEntry> volumes;

    std::vector<OrderBookEntry> orders = orderBook.getOrders(OrderBookType::unknown, product);
    int nextTimeCounter = 0;
    std::string currentTime = orderBook.getEarliestTime(); // deberia cambiarlo al de orders[0]?

    double asksVolume = 0;
    double bidsVolume = 0;
    for (OrderBookEntry &order : orders) {
        double volume = order.price * order.amount;
        
        if (order.timestamp == currentTime) {
            if (order.orderType == OrderBookType::ask) {
                asksVolume += volume;
            } else if (order.orderType == OrderBookType::bid) {
                bidsVolume += volume;
            }
        } else if (nextTimeCounter <= period) {
            if (order.orderType == OrderBookType::ask) {
                asksVolume += volume;
            } else if (order.orderType == OrderBookType::bid) {
                bidsVolume += volume;
            }

            currentTime = order.timestamp;
            nextTimeCounter += 1;
        } else {
            VolumeEntry volumeEntry = VolumeEntry(currentTime, asksVolume, bidsVolume);
            volumes.push_back(volumeEntry);

            asksVolume = 0;
            bidsVolume = 0;
        
            if (order.orderType == OrderBookType::ask) {
                asksVolume += volume;
            } else if (order.orderType == OrderBookType::bid) {
                bidsVolume += volume;
            }

            currentTime = order.timestamp;
            nextTimeCounter = 0;
        }

    }

    return volumes;
}

void VolumeGraph::plot() {
    int graphWidth = volumes.size() * 11 + 14;

    for (int i = 0; i < graphWidth; i++) {
        std::cout << "═";
    }
    std::cout << std::endl;

    std::cout << MAGENTA << "██ ask" << RESET << " | " << CYAN << "██ bid" << RESET << " | " << period * 5 / 60 << " minutes" << std::endl;
    
    for (int i = 0; i < graphWidth; i++) {
        std::cout << "═";
    }
    std::cout << std::endl;

    std::vector<double> Yvalues = getOrderedValues();

    for (double &y : Yvalues) {
        std::cout << std::setw(12) << y << " │";

        for (VolumeEntry &volume : volumes) {
            if (y <= volume.ask && y <= volume.bid) {
                std::cout << MAGENTA << "   ██" << CYAN << "██    ";
            } else if (y <= volume.ask && y > volume.bid) {
                std::cout << MAGENTA << "   ██      ";
            } else if (y > volume.ask && y <= volume.bid) {
                std::cout << CYAN << "     ██    ";
            } else {
                std::cout << "           ";
            }
            
        }
        std::cout << RESET << std::endl;
    }

    for (int i = 0; i < graphWidth; i++) {
        std::cout << "▔";
    }
    std::cout << std::endl;

    std::vector<std::string> timeframes = getTimeframes();
    std::cout <<  "             | ";
    for (std::string timeframe : timeframes) {
        std::cout << timeframe << "   ";
    }
    std::cout << std::endl;
}

std::vector<double> VolumeGraph::getOrderedValues() {
    std::vector<double> values;

    for (VolumeEntry &volume : volumes) {
        values.push_back(volume.ask);
        values.push_back(volume.bid);
    }

    std::sort(values.begin(), values.end(), std::greater<double>());
    return values;
}

std::vector<std::string> VolumeGraph::getTimeframes() {
    std::vector<std::string> timeframes;

    for (VolumeEntry &volume : volumes) {
        std::string time = volume.timestamp.substr(11, 8);
        timeframes.push_back(time);
    }

    return timeframes;
}
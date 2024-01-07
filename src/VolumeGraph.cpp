#include "VolumeGraph.h"
#include "TerminalWindow.h"
#include <iostream>
#include <iomanip>
#include <set>

// terminal output colors
#define RESET   "\033[0m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"

VolumeGraph::VolumeGraph(OrderBook &orderBook, std::string &_product, int &_period) {
    product = _product;
    period = _period;
    volumes = getVolumes(orderBook);
}

std::vector<VolumeEntry> VolumeGraph::getVolumes(OrderBook &orderBook) {
    std::vector<VolumeEntry> volumes;

    // get orders with any order type for given product
    std::vector<OrderBookEntry> orders = orderBook.getOrders(OrderBookType::unknown, product);
    
    // set next time counter to 0. This counter checks if the order is in our current time period
    int nextTimeCounter = 0;

    // get earliest time as current time
    std::string currentTime = orderBook.getEarliestTime();

    // set start time as current time
    std::string startTime = currentTime;

    // set initial asks and bids volume to 0
    double asksVolume = 0;
    double bidsVolume = 0;

    // iterate through orders
    for (const OrderBookEntry &order : orders) {
        // calculate order entry volume
        double volume = order.price * order.amount;
        
        if (order.timestamp == currentTime) { // if order is in current time period, add its volume to asks or bids volume
            if (order.orderType == OrderBookType::ask) {
                asksVolume += volume;
            } else if (order.orderType == OrderBookType::bid) {
                bidsVolume += volume;
            }
        } else if (nextTimeCounter <= period) { // if order is still in current time period, add its volume to asks or bids volume and increment next time counter
            if (order.orderType == OrderBookType::ask) {
                asksVolume += volume;
            } else if (order.orderType == OrderBookType::bid) {
                bidsVolume += volume;
            }

            // update current time and increment next time counter
            currentTime = order.timestamp;
            nextTimeCounter += 1;
        } else { // if order is not in current time period, create volume entry with current period volume and add it to volumes vector
            VolumeEntry volumeEntry{startTime, currentTime, asksVolume, bidsVolume};
            volumes.push_back(volumeEntry);

            // reset current period volumes values
            asksVolume = 0;
            bidsVolume = 0;
        
            // add current order volume to asks or bids volume
            if (order.orderType == OrderBookType::ask) {
                asksVolume += volume;
            } else if (order.orderType == OrderBookType::bid) {
                bidsVolume += volume;
            }

            // update current time, update start time and reset next time counter
            currentTime = order.timestamp;
            startTime = order.timestamp;
            nextTimeCounter = 0;
        }
    }

    // create volume entry with the orders that are left, if there are any
    if (asksVolume > 0 || bidsVolume > 0) {
        VolumeEntry volumeEntry{startTime, currentTime, asksVolume, bidsVolume};
        volumes.push_back(volumeEntry);
    }

    return volumes;
}

void VolumeGraph::plot() {
    // get graph width and console width
    int graphWidth = volumes.size() * 11 + 14;
    int consoleWidth = TerminalWindow::getConsoleWidth();
    
    // check if graph can be displayed in console, else display error message
    if (graphWidth > consoleWidth) {
        TerminalWindow::error(
            "Can't display graph in this console, try a wider console with at least " + std::to_string(graphWidth) + ". Current width " + std::to_string(consoleWidth)
        );
        return;
    }

    // print graph header
    for (int i = 0; i < graphWidth; i++) {
        std::cout << "═";
    }
    std::cout << std::endl;

    std::cout << product << " | " << MAGENTA << "██ ask" << RESET << " | " << CYAN << "██ bid" << RESET << " | " << period * 5 / 60 << " minutes" << std::endl;
    
    for (int i = 0; i < graphWidth; i++) {
        std::cout << "═";
    }
    std::cout << std::endl;

    // get Y axis values
    std::vector<double> Yvalues = getOrderedValues();

    // iterate through Y values
    for (const double &y : Yvalues) {
        // print Y value
        std::cout << std::setw(12) << y << " │";

        // iterate through volume entries to check if they are in the current Y value
        for (const VolumeEntry &volume : volumes) {
            if (y <= volume.ask && y <= volume.bid) { // if both ask and bid values are higher than Y value, print both with their colors
                std::cout << MAGENTA << "   ██" << CYAN << "██    ";
            } else if (y <= volume.ask && y > volume.bid) { // if only ask value is higher than Y value, print ask with its color
                std::cout << MAGENTA << "   ██      ";
            } else if (y > volume.ask && y <= volume.bid) { // if only bid value is higher than Y value, print bid with its color
                std::cout << CYAN << "     ██    ";
            } else { // if none of the values are higher than Y value, print empty space
                std::cout << "           ";
            }
            
        }
        std::cout << RESET << std::endl;
    }

    // print graph bottom
    for (int i = 0; i < graphWidth; i++) {
        std::cout << "▔";
    }
    std::cout << std::endl;

    // print X axis values with start times
    std::vector<std::string> startTimes = getStartTimes();
    std::cout <<  "             │ ";
    for (const std::string &timeframe : startTimes) {
        std::cout << timeframe << "   ";
    }
    std::cout << std::endl;

    // print end times right below start times
    std::vector<std::string> endTimes = getEndTimes();
    std::cout <<  "             │ ";
    for (const std::string &timeframe : endTimes) {
        std::cout << timeframe << "   ";
    }
    std::cout << std::endl;
}

std::vector<double> VolumeGraph::getOrderedValues() {
    std::vector<double> values;

    // get all ask and bid values from volume entries
    for (const VolumeEntry &volume : volumes) {
        values.push_back(volume.ask);
        values.push_back(volume.bid);
    }

    // use set to automatically remove duplicates and asign values to that set
    std::set<double> uniqueValues(values.begin(), values.end());
    values.assign(uniqueValues.begin(), uniqueValues.end());

    // sort values in descending order
    std::sort(values.begin(), values.end(), std::greater<double>());

    return values;
}

std::vector<std::string> VolumeGraph::getStartTimes() {
    std::vector<std::string> timestamps;

    // get every volume entry start time
    for (const VolumeEntry &volume : volumes) {
        std::string time = volume.startTime.substr(11, 8);
        timestamps.push_back(time);
    }

    return timestamps;
}

std::vector<std::string> VolumeGraph::getEndTimes() {
    std::vector<std::string> timestamps;

    // get every volume entry end time
    for (const VolumeEntry &volume : volumes) {
        std::string time = volume.endTime.substr(11, 8);
        timestamps.push_back(time);
    }

    return timestamps;
}
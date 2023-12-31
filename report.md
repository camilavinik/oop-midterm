# TASK 1: Compute candlestick data
## Challenges and way of working
This first task was personally the most challenging for me, as I changed multiple times the way the data was processed when encountered problems with optimization. But they were all in the same idea of iterating over the order book and creating a candlestick every x amount of time.

The first working version used functions like ````OrderBook::getNextTime```` to “travel” through time and get several orders in a period of time using ````OrderBook::getOrders````. This strategy worked fine but it took a few minutes to return a vector of candlesticks. It was not a pleasant experience.

I then jumped to the conclusion that using ````OrderBook::getNextTime```` and ````OrderBook::getOrders```` was iterating multiple and undesired times through the order book which was in this case, pretty big. Maybe a shorter dataset would have worked fast enough, but I wanted the algorithm to be fast enough to target datasets.

A refactor was made avoiding these two functions and instead, iterating once in the order book, changing the behavior of ````OrderBook::getOrders```` to be able to retrieve orders on any timestamp if no timestamp was being passed into the function. And ````OrderBook::getNextTime```` was replaced manually checking if the timestamp I am currently on is different from the one before.

SCRUM methodology was used for the creation of this project, going through the most basic case to the most complex. 
For this first task, the time period of each candlestick was set to 15 minutes, and the order type and product were hard-coded into the code for a specific product and type.
This is why the code changed for Task 2. Although the logic and main components were the same, functions were rearranged into different classes, and a more customizable experience was created for the user to manually input the desired type, product, and time period.

I wanted Task 1 to be as simple as possible and have more challenges when Task 2 was implemented. More about how the code was organized and the logic behind the algorithm can be found in the next section.

## Code and Logic explanation
We first have our Candlestick class, this is a very simple class just with a constructor function, and the requested ````date````, ````open````, ````high````, ````low````, and close values.

The Candlestick constructor receives a vector of ````OrderBookEntry```` and a double open, being this last directly the open value for the created candlestick.

On the constructor function, we iterate through the order book entries to find out the highest value, and lowest value, and calculate the total amount and value to determine its close value.

Now for getting a whole vector of candlesticks, I defined a function that would be later moved ````CandlestickGraph```` class (you will find it in the code there) for object-oriented programming purposes, but for this task, this function was first developed in the ````MerkelMain```` class.

The candlestick vector is constructed by iterating through multiple orders and dividing them into periods, every period will at the end, have its own candlestick, and every period at this time of development corresponds to 15 minutes. As the data we have changed its timestamp every 5 seconds, we calculate 15 minutes as 180 timestamps, so when we reach that number, a period is completed and a candlestick is created. 

Note that this period number will be changed for task 2, as it becomes customizable.

Candlesticks vector is processed this way:
We first define our current time as the earliest time on the order book.
We get a vector of ````OrderBookEntry```` with all orders for a specific type and product but for any timestamp.
We define the first open value as the first order price. This was a personal decision as we don’t have more information about the first candlestick.
We set out timestamp counter ````nextTimeCounter```` at 0

We now iterate through the orders, we know that if the order being processed has the same timestamp as my current time, then will be for sure in our time period. So we add this order to our current orders vector. Remember this vector will be passed to create a candlestick out of the entries we select.

A second case we can encounter is that the timestamp is different from our current time, but if the timestamp counter is less than our period, then we also know it belongs to our current period. We do the same as the first case, and in addition, we update our current time and our nextTimeCounter.

The last case is when none of these options happens, then it means the current order is outside the period and, therefore is the first order from the next period. We will first of all create a candlestick out of all the order pushed into our current orders vector with the open value we got defined, and push that candlestick into our final candlestick vector. 
Then we set the open value to this recently created candlestick close value, and this will be used the next candlestick is created. We also reset our current orders and leave just the one we are iterating in because as we said before, it belongs in the next period. We update our current time and reset our timestamp counter.

This process goes on until we reach the end of the order book. So after that, we check if there are any current orders left, because if we didn’t switch the period then it wasn’t created, and is expected from this last period to be incomplete. Therefore we create a candlestick out of what we have left.

Then we simply return our candlesticks vector.

# TASK 2: Create a text-based plot of the candlestick data
## Challenges and way of working
Task 2 was implemented out of Task 1 base case, this is, the period was 15 minutes, and order type and product were hardcoded into the code, as I used the Task 1 first version result for plotting the graph.

Improvements and creativity were implemented once Task 2 was completed, this is why I created a section “Improvements” to talk about how I have enhanced the app experience by covering multiple scenarios and user needs. So once again the code explained below may be a little bit different from the actual provided code, but will make more sense after the Improvements sections.

In terms of challenges, I found it a little bit hard at the beginning to think about how could I create a text-based graph. But ended up being a simple logic of “one row at a time”.

I still needed to investigate how to print colors in the console and which characters could I use so it was more eye-appealing, I ended up using color codes that changed the console output once used, and I also chose characters that occupied the whole character space to avoid empty spaces.

Also for improvements, I got to investigate how to calculate the width of the output terminal, and I got my solution out of a stack overflow thread. 
This was: https://stackoverflow.com/questions/1022957/getting-terminal-width-in-c

## Code and Logic explanation
We start by creating a new class ````CandlestickGraph````, where we have the constructor function that creates our candlestick vector data out of order book, type, product, and period, which are all saved in this class.

To create the candlestick vector, the code mentioned in Task 1 was moved into our new class as a private function ````CandlestickGraph::getCandlesticks````.

We also create two more private functions ````CandlestickGraph::getOrderedValues```` and ````CandlestickGraph::getTimeframes````, which will retrieve respectively our Y-axis values and X-axis values, with the slight difference that ````CandlestickGraph::getOrderedValues```` will return values in descending order, as we want the highest value at the top.

Then we have a plot function ````CandlestickGraph::plot````, which will be the one to output in the console our graph out of the data on the class.

We start by calculating our graph width, which is eleven times the number of candlesticks, as each candlestick occupies 11 spaces, plus 14, which is the size of our Y-axis values.

We use our calculated graph width to print the header, where we also print our period, type, and product information.

We use ````CandlestickGraph::getOrderedValues```` to get all numeric values of the candlesticks in descending order, and then iterate through them.

In each value, we will be checking for each candlestick if any of its values are lower than Y, so for example if the Y value is between some candlestick open and close value, then we will be displaying the wider part of the candlestick.
If it's not between them but is between its low and high value then we print the thinner part of the candlestick.
And if none of these options happens, then we print empty spaces.
After all the candlesticks were analyzed for and Y-value, we do a line jump and check for the next Y-value.

It is also important to mention that every time we print something on this step, it's eleven spaces wide, so the result is consistent.

We also have colors for each candlestick, red or green depending if our close value is lower than our open value, and the color reset for each candlestick.

After we finish plotting, we just add the graph footer using our graph width, and get the timeframes out of ````CandlestickGraph::getTimeframes````, which will be eleven spaces each once again.

## Improvements
The first and most needed improvement, not only for Task 2 but for Task 1, was the ability to enter order type, product, and period.

This was made in ````MerkelMain```` with its own menu option ````MerkelMain::printCandlesticks````, we ask for every item listed above and check if the input of the user makes sense, otherwise, we print an error message.

For the period is important to notice that the user enters the input in minutes, but is converted and saved in “next time jumps”, calculating: minutes * 60 / 5. But if the user does not have a preferred time period, or an invalid value is input, our 15-minute default will be applied.

With this custom period option, the graph may get extremely wide so some resolutions won't be able to see the graph as it should, so I created the TerminalWindow class with ````TerminalWindow::getConsoleWidth```` function, so we could calculate the console width not only for Task 2 but for also Task 3.

With this new class created, I also decided to create a print error function and a print message function, these would print a passed-into message either in red or in bold, depending on if it is an error or just a message. These functions were implemented in ````MerkelMain::printCandlesticks````, to improve the user experience when communicating through the console.

Also, I came up with the idea of whether the user may want to see the first candlestick or not, as the first candlestick does not have a specific open value, it would be expected for some users to avoid it, or simply not want to see it. On the other hand, some users may still want to see it with the open value as the first order price, as it is the first value of the candlestick. This decision is communicated to the user so the user can decide if the graph will show the first candlestick or not.

# TASK 3: Plot a text graph of some other trading data
## Challenges and way of working
This task was the least challenging of all, as plotting was already solved for Task 2, and analyzing data was solved for Task 1, therefore the main challenge was encountering trading data some users would want to know.

I wanted to show a different type of graph this time, I wanted a graph that would show asks and bids at the same time so I could play a little bit more with colors and show multiple values at the same time.

I have then decided to go with a Value Graph that would show the amount of asks and bids in a specified time period, for a desired product.

For this task, I also decided to use the period’s end time, as now the graph data will be represented as columns more apart from each other than candlesticks, plus adding another challenge.

## Code and Logic explanation
This explanation will be considerably shorter than the other ones, as most of the code logic was reused.

Similarly to the candlestick, we first have our ````VolumeEntry```` class, where we store the information for one specific period of time, where we have ````startTime````, ````endTime````, ````ask```` and ````bid````, being these last two the value amount for each.

Then we have our ````VolumeGraph```` class, where we have ````VolumeGraph::getVolumes```` function to calculate our data, this is done very similarly to ````CandlestickGraph::getCandlesticks````.

We get all orders for a specific time, with any timestamp and any product type, we also set ````nextTimeCounter```` and ````currentTime```` as we did for candlesticks, and we also set initial values for ````asksVolume````, ````bidsVolume```` and ````startTime````.

We iterate over the order book adding each bid or ask value to its corresponding ````asksVolume```` or ````bidsVolume````, following the same period rules as we did for Task 2.

Once a period is completed we just create a ````VolumeEntry```` with the data collected and we reset those values and start counting again.

Lastly, we create the volume entry with the remaining data and return a ````VolumeEntry```` vector.

We also have ````VolumeGraph::plot```` working almost the same as ````CandlestickGraph::plot````, with the only difference that for each Y-axis value, we will render either the asks bar, the bids bar, both, or none, depending if these values are higher than the current Y or not.

To use this function we also created a function in ````MerkelMain```` class ````MerkelMain::printVolumeGraph````, that asks for the desired product and a time period the same way we did for Task 2.
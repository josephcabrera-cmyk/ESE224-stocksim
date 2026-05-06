#include "GoldenCrossStrategy.h"
<<<<<<< HEAD
#include <string>
using namespace std;

// Helper functions only used inside this file
static int getYearFromDate(const string& date) {
    if (date.length() < 4) {
        return -1;
    }
    return stoi(date.substr(0, 4));
}

static int getMonthFromDate(const string& date) {
    if (date.length() < 7) {
        return -1;
    }
    return stoi(date.substr(5, 2));
}

GoldenCrossStrategy::GoldenCrossStrategy(int shortWindow, int longWindow)
    : shortWindow(shortWindow), longWindow(longWindow) {
    
    if (this->shortWindow <= 0) {
        this->shortWindow = 50;
    }

    if (this->longWindow <= 0) {
        this->longWindow = 200;
    }

    if (this->shortWindow >= this->longWindow) {
        this->shortWindow = 50;
        this->longWindow = 200;
    }
}

string GoldenCrossStrategy::getName() const {
    return "Golden Cross (50/200 MA)";
}

SimResult GoldenCrossStrategy::backtest(PriceHistory* history,
                                        double monthlyCapital,
                                        int startYear,
                                        int endYear) {
    SimResult result;
    result.strategyName = getName();
    result.finalValue = 0.0;
    result.totalInvested = 0.0;
    result.totalReturn = 0.0;
    result.cagr = 0.0;
    result.maxDrawdown = 0.0;
    result.totalTrades = 0;

    if (history == nullptr || history->getSize() == 0) {
        return result;
    }

    CircularQueue ma50Queue(shortWindow);
    CircularQueue ma200Queue(longWindow);

    double cash = 0.0;
    double shares = 0.0;

    bool inPosition = false;
    bool hasPreviousMA = false;

    double prevMA50 = 0.0;
    double prevMA200 = 0.0;

    int lastContributionYear = -1;
    int lastContributionMonth = -1;

    vector<double> portfolioValues;

    for (PriceHistory::Iterator it = history->begin(); it != history->end(); ++it) {
        PriceNode& node = *it;

        int year = getYearFromDate(node.date);
        int month = getMonthFromDate(node.date);

        if (year < startYear || year > endYear) {
            continue;
        }

        double closePrice = node.close;

        // Add monthly capital on the first available trading day of each month
        if (year != lastContributionYear || month != lastContributionMonth) {
            cash += monthlyCapital;
            result.totalInvested += monthlyCapital;

            lastContributionYear = year;
            lastContributionMonth = month;

            // If already in position, invest the new monthly cash immediately
            if (inPosition && cash > 0.0 && closePrice > 0.0) {
                shares += cash / closePrice;
                cash = 0.0;
                result.totalTrades++;
            }
        }

        // Update moving average queues
        ma50Queue.enqueue(closePrice);
        ma200Queue.enqueue(closePrice);

        // Need both queues full before using crossover signals
        if (ma50Queue.isFull() && ma200Queue.isFull()) {
            double currMA50 = ma50Queue.getAverage();
            double currMA200 = ma200Queue.getAverage();

            if (hasPreviousMA) {
                bool goldenCross = (prevMA50 <= prevMA200 && currMA50 > currMA200);
                bool deathCross = (prevMA50 >= prevMA200 && currMA50 < currMA200);

                // Buy signal: 50-day MA crosses above 200-day MA
                if (goldenCross && !inPosition && cash > 0.0 && closePrice > 0.0) {
                    shares = cash / closePrice;
                    cash = 0.0;
                    inPosition = true;
                    result.totalTrades++;
                }

                // Sell signal: 50-day MA crosses below 200-day MA
                else if (deathCross && inPosition && shares > 0.0) {
                    cash = shares * closePrice;
                    shares = 0.0;
                    inPosition = false;
                    result.totalTrades++;
                }
            }

            prevMA50 = currMA50;
            prevMA200 = currMA200;
            hasPreviousMA = true;
        }

        double portfolioValue = cash + shares * closePrice;
        portfolioValues.push_back(portfolioValue);
    }

    // Use last available price in range for final portfolio value
    double finalPrice = 0.0;

    for (PriceHistory::ReverseIterator it = history->rbegin(); it != history->rend(); ++it) {
        PriceNode& node = *it;
        int year = getYearFromDate(node.date);

        if (year >= startYear && year <= endYear) {
            finalPrice = node.close;
            break;
        }
    }

    result.finalValue = cash + shares * finalPrice;

    if (result.totalInvested > 0.0) {
        result.totalReturn =
            (result.finalValue - result.totalInvested) / result.totalInvested * 100.0;
    }

    result.cagr = calculateCAGR(result.totalInvested,
                                result.finalValue,
                                endYear - startYear);

    result.maxDrawdown = calculateMaxDrawdown(portfolioValues);

    return result;
=======
using namespace std;

GoldenCrossStrategy::GoldenCrossStrategy(int shortWindow, int longWindow)
    : shortWindow(shortWindow), longWindow(longWindow) {}

SimResult GoldenCrossStrategy::backtest(PriceHistory* history,
double monthlyCapital, int startYear, int endYear) {

    SimResult result{};
    result.strategyName = getName();

    if (history == nullptr) {
        return result;
    }

    double shares = 0.0;
    double cash = monthlyCapital;
    double totalInvested = monthlyCapital;
    vector<double> portfolioValues;

    CircularQueue ma50Queue(shortWindow);
    CircularQueue ma200Queue(longWindow);
    double prev_ma50 = 0.0;
    double prev_ma200 = 0.0;
    bool prevSet = false;

    PriceNode* current = history->getHead();

    while (current != nullptr) {
        int nodeYear = stoi(current->date.substr(0, 4));

        if (nodeYear >= startYear && nodeYear <= endYear) {
            ma50Queue.enqueue(current->close);
            ma200Queue.enqueue(current->close);
            if (ma50Queue.isFull() && ma200Queue.isFull()) {
                double curr_ma50 = ma50Queue.getAverage();
                double curr_ma200 = ma200Queue.getAverage();

                if (prevSet) {
                    if (prev_ma50 <= prev_ma200 && curr_ma50 > curr_ma200 && cash > 0) { // golden cross
                        shares += cash / current->close;
                        cash = 0;
                        result.totalTrades++;
                    }

                    else if (prev_ma50 >= prev_ma200 && curr_ma50 < curr_ma200 && shares > 0) { // death cross
                        cash += shares * current->close;
                        shares = 0;
                        result.totalTrades++;
                    }
                }
                prev_ma50 = curr_ma50;
                prev_ma200 = curr_ma200;
                prevSet = true;
            }
            portfolioValues.push_back(shares * current->close + cash);
        }
        current = current->next;
    }
    result.totalInvested = totalInvested;

    if (!portfolioValues.empty()) {
        result.finalValue = portfolioValues.back();
        result.maxDrawdown = calculateMaxDrawdown(portfolioValues);
        result.totalReturn = (result.finalValue - result.totalInvested) / result.totalInvested * 100.0;
        result.cagr = calculateCAGR(portfolioValues.front(), portfolioValues.back(), endYear - startYear + 1);
    }

    return result;
}

string GoldenCrossStrategy::getName() const { 
    return "Golden Cross (50/200 MA)";
>>>>>>> de251ab6ced52a8bdef6f55a150c7fbf59eca2c8
}
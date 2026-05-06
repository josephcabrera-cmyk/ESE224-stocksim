#include "MomentumStrategy.h"
#include <string>
using namespace std;

// Helper functions only used in this file
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

MomentumStrategy::MomentumStrategy(double momentumThreshold, int lookbackDays)
    : momentumThreshold(momentumThreshold), lookbackDays(lookbackDays) {
    
    if (this->lookbackDays <= 0) {
        this->lookbackDays = 126;
    }
}

string MomentumStrategy::getName() const {
    return "6-Month Momentum";
}

SimResult MomentumStrategy::backtest(PriceHistory* history,
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

    double cash = 0.0;
    double shares = 0.0;
    bool inPosition = false;

    int lastMonth = -1;
    int lastYear = -1;

    vector<double> portfolioValues;

    for (PriceHistory::Iterator it = history->begin(); it != history->end(); ++it) {
        PriceNode& currentNode = *it;

        int year = getYearFromDate(currentNode.date);
        int month = getMonthFromDate(currentNode.date);

        if (year < startYear || year > endYear) {
            continue;
        }

        double currentPrice = currentNode.close;

        // Record daily portfolio value for drawdown
        double dailyValue = cash + shares * currentPrice;
        portfolioValues.push_back(dailyValue);

        // Only evaluate/rebalance once per month
        if (year == lastYear && month == lastMonth) {
            continue;
        }

        lastYear = year;
        lastMonth = month;

        // Use ReverseIterator to find current node, then go backward lookbackDays
        PriceNode* lookbackNode = nullptr;
        bool foundCurrent = false;
        int daysBack = 0;

        for (PriceHistory::ReverseIterator rit = history->rbegin();
             rit != history->rend();
             ++rit) {
            
            PriceNode& node = *rit;

            if (!foundCurrent) {
                if (node.date == currentNode.date) {
                    foundCurrent = true;
                }
            } else {
                daysBack++;

                if (daysBack >= lookbackDays) {
                    lookbackNode = &node;
                    break;
                }
            }
        }

        // If there is not enough history yet, skip this month
        if (lookbackNode == nullptr || lookbackNode->close <= 0.0) {
            continue;
        }

        double oldPrice = lookbackNode->close;
        double trailingReturn = (currentPrice - oldPrice) / oldPrice * 100.0;

        // Buy / stay invested if momentum is strong
        if (trailingReturn > momentumThreshold) {
            cash += monthlyCapital;
            result.totalInvested += monthlyCapital;

            if (currentPrice > 0.0) {
                shares += cash / currentPrice;
                cash = 0.0;
                inPosition = true;
                result.totalTrades++;
            }
        }

        // Sell if momentum turns negative
        else if (trailingReturn < 0.0) {
            if (inPosition && shares > 0.0) {
                cash += shares * currentPrice;
                shares = 0.0;
                inPosition = false;
                result.totalTrades++;
            }

            // No new investment during negative momentum
        }

        // Between 0 and threshold: hold current position, no new investment
        else {
            // Do nothing
        }
    }

    // Find final price inside year range
    double finalPrice = 0.0;

    for (PriceHistory::ReverseIterator it = history->rbegin();
         it != history->rend();
         ++it) {
        
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
}
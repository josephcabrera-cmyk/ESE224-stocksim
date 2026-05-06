#include "GoldenCrossStrategy.h"
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
}
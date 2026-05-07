#include "FixedSIPStrategy.h"

SimResult FixedSIPStrategy::backtest(PriceHistory* history,
double monthlyCapital, int startYear, int endYear) {
    
    SimResult result{};
    result.strategyName = getName();

    if (history == nullptr) {
        return result;
    }

    double totalInvested = 0.0;
    double shares = 0.0;
    vector<double> portfolioValues;

    for (int year = startYear; year <= endYear; year++) {
        for (int month = 1; month <= 12; month++) {

            PriceNode* current = history->getHead();

            while (current != nullptr) {
                int nodeYear = stoi(current->date.substr(0, 4));
                int nodeMonth = stoi(current->date.substr(5, 2));

                if (nodeYear == year && nodeMonth == month) {
                    double closePrice = current->close;

                    double sharesBought = monthlyCapital / closePrice;
                    shares += sharesBought;
                    totalInvested += monthlyCapital;

                    double portfolioValue = shares * closePrice;

                    result.totalInvested = totalInvested;
                    result.finalValue = portfolioValue;
                    result.totalTrades++;
                    portfolioValues.push_back(portfolioValue);

                    break;
                }

                current = current->next;
            }
        }
    }

    if (!portfolioValues.empty()) {
        result.maxDrawdown = calculateMaxDrawdown(portfolioValues);
        result.totalReturn = (result.finalValue - result.totalInvested) / result.totalInvested * 100.0;
        result.cagr = calculateCAGR(totalInvested, result.finalValue, endYear - startYear + 1);
    }

    return result;
}

string FixedSIPStrategy::getName() const {
    return "Fixed SIP";
}
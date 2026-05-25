#include "FixedSIPStrategy.h"

static void updateMaxDrawdown(double currentValue, double& peakValue, double& maxDrawdown) {
    if (currentValue > peakValue) {
        peakValue = currentValue;
    }

    if (peakValue > 0.0) {
        double drawdown = (peakValue - currentValue) / peakValue * 100.0;

        if (drawdown > maxDrawdown) {
            maxDrawdown = drawdown;
        }
    }
}

SimResult FixedSIPStrategy::backtest(PriceHistory* history,
double monthlyCapital, int startYear, int endYear) {
    
    SimResult result{};
    result.strategyName = getName();

    if (history == nullptr) {
        return result;
    }

    double totalInvested = 0.0;
    double shares = 0.0;
    double peakValue = 0.0;
    double maxDrawdown = 0.0;
    bool hasPortfolioValue = false;

    for (int year = startYear; year < endYear; year++) {
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
                    updateMaxDrawdown(portfolioValue, peakValue, maxDrawdown);
                    hasPortfolioValue = true;

                    break;
                }

                current = current->next;
            }
        }
    }

    if (hasPortfolioValue) {
        double finalPrice = 0.0;

        for (PriceHistory::ReverseIterator it = history->rbegin(); it != history->rend(); ++it) {
            PriceNode& node = *it;
            int year = stoi(node.date.substr(0, 4));

            if (year >= startYear && year < endYear) {
                finalPrice = node.close;
                break;
            }
        }

        if (finalPrice > 0.0) {
            result.finalValue = shares * finalPrice;
            updateMaxDrawdown(result.finalValue, peakValue, maxDrawdown);
        }

        result.maxDrawdown = maxDrawdown;
        if (result.totalInvested > 0.0) {
            result.totalReturn = (result.finalValue - result.totalInvested) / result.totalInvested * 100.0;
            result.cagr = calculateCAGR(totalInvested, result.finalValue, endYear - startYear);
        }
    }

    return result;
}

string FixedSIPStrategy::getName() const {
    return "Fixed SIP";
}

#include "DynamicSIPStrategy.h"
using namespace std;

DynamicSIPStrategy::DynamicSIPStrategy(double dipThreshold, double rallyThreshold, double multiplier)
    : dipThreshold(dipThreshold), rallyThreshold(rallyThreshold), multiplier(multiplier) {}

SimResult DynamicSIPStrategy::backtest(PriceHistory* history,
double monthlyCapital, int startYear, int endYear) {

    SimResult result{};
    result.strategyName = getName();

    if (history == nullptr) {
        return result;
    }

    double totalInvested = 0.0;
    double shares = 0.0;
    vector<double> portfolioValues;
    vector<double> recentPrices;

    for (int year = startYear; year <= endYear; year++) {
        
        for (int month = 1; month <= 12; month++) {

            PriceNode* current = history->getHead();

            while (current != nullptr) {
                int nodeYear = stoi(current->date.substr(0, 4));
                int nodeMonth = stoi(current->date.substr(5, 2));

                if (nodeYear == year && nodeMonth == month) {
                    double closePrice = current->close;

                    double spend = monthlyCapital;
                    if (recentPrices.size() >= 12) {
                        double high = recentPrices[0];
                        double low = recentPrices[0];
                        for (size_t i = 1; i < recentPrices.size(); i++) {
                            if (recentPrices[i] > high) {
                                high = recentPrices[i];
                            }
                            if (recentPrices[i] < low) {
                                low = recentPrices[i];
                            }
                        }
                        double dipFromHigh = (high - closePrice) / high * 100;
                        double rallyFromLow = (closePrice - low) / low * 100;

                        if (dipFromHigh >= dipThreshold) {
                            spend = multiplier * monthlyCapital;
                        }
                        else if (rallyFromLow >= rallyThreshold) {
                            spend = 0.5 * monthlyCapital;
                        }
                    }

                    double totalBudget = (endYear - startYear + 1) * 12 * monthlyCapital;
                    if (totalInvested + spend > totalBudget) {
                        spend = totalBudget - totalInvested;
                    }

                    if (spend > 0) {
                        double sharesBought = spend /closePrice;
                        shares += sharesBought;
                        totalInvested += spend;

                        double portfolioValue = shares * closePrice;

                        result.totalInvested = totalInvested;
                        result.finalValue = portfolioValue;
                        result.totalTrades++;
                        portfolioValues.push_back(portfolioValue);
                    }

                    recentPrices.push_back(closePrice);
                    if (recentPrices.size() > 12) {
                        recentPrices.erase(recentPrices.begin());
                    }

                    break;
                }

                current = current->next;
            }
        }
    }

    if (!portfolioValues.empty()) {
        result.maxDrawdown = calculateMaxDrawdown(portfolioValues);
        result.totalReturn = (result.finalValue - result.totalInvested) / result.totalInvested * 100.0;
        result.cagr = calculateCAGR(portfolioValues.front(), portfolioValues.back(), endYear - startYear + 1);
    }

    return result;
}

string DynamicSIPStrategy::getName() const { 
    return "Dynamic SIP";
}

double DynamicSIPStrategy::getDipThreshold() const { 
    return dipThreshold;
}

double DynamicSIPStrategy::getRallyThreshold() const {
    return rallyThreshold;
}

double DynamicSIPStrategy::getMultiplier() const {
    return multiplier;
}
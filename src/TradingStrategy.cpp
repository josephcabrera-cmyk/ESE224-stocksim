#include "TradingStrategy.h"
#include <iostream>
#include <cmath>
using namespace std;

void TradingStrategy::printResult(const SimResult& result) const {
    cout << "Strategy: " << result.strategyName << endl;
    cout << "Final Value: $" << result.finalValue << endl;
    cout << "Total Invested: $" << result.totalInvested << endl;
    cout << "Total Return: " << result.totalReturn << "%" << endl;
    cout << "CAGR: " << result.cagr << "%" << endl;
    cout << "Max Drawdown: " << result.maxDrawdown << "%" << endl;
    cout << "Total Trades: " << result.totalTrades << endl;
}

double TradingStrategy::calculateCAGR(double startVal, double endVal, int years) const {
    if (years <= 0 || startVal <= 0 || endVal <= 0) {
        return 0.0;
    }

    double cagr = pow(endVal / startVal, 1.0 / years) - 1.0;

    return cagr * 100.0;
}

double TradingStrategy::calculateMaxDrawdown(const vector<double>& portfolioValues) const {
    if (portfolioValues.empty()) {
        return 0.0;
    }

    double peak = portfolioValues[0];
    double maxDrawdown = 0.0;

    for (int i = 0; i < (int)portfolioValues.size(); i++) {
        double currentValue = portfolioValues[i];

        if (currentValue > peak) {
            peak = currentValue;
        }

        if (peak > 0) {
            double drawdown = (peak - currentValue) / peak * 100.0;

            if (drawdown > maxDrawdown) {
                maxDrawdown = drawdown;
            }
        }
    }

    return maxDrawdown;
}
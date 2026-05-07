#include "TradingStrategy.h"
#include <iostream>
using namespace std;

static bool powerAtLeast(double base, int exponent, double target) {
    double result = 1.0;

    for (int i = 0; i < exponent; i++) {
        result *= base;

        if (base >= 1.0 && result >= target) {
            return true;
        }
    }

    return result >= target;
}

static double nthRoot(double value, int n) {
    if (value <= 0.0 || n <= 0) {
        return 0.0;
    }

    double low = 0.0;
    double high = (value >= 1.0) ? value : 1.0;

    for (int i = 0; i < 100; i++) {
        double mid = (low + high) / 2.0;

        if (powerAtLeast(mid, n, value)) {
            high = mid;
        }
        else {
            low = mid;
        }
    }

    return (low + high) / 2.0;
}

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

    double cagr = nthRoot(endVal / startVal, years) - 1.0;

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

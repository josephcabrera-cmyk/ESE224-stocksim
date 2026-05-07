#include "MomentumStrategy.h"
using namespace std;

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

MomentumStrategy::MomentumStrategy(double momentumThreshold, int lookbackDays)
	: momentumThreshold(momentumThreshold), lookbackDays(lookbackDays) {}

SimResult MomentumStrategy::backtest(PriceHistory* history,
double monthlyCapital, int startYear, int endYear) {
	SimResult result{};
	result.strategyName = getName();
	if (history == nullptr) {
		return result;
	}
	double shares = 0.0;
	double cash = 0.0;
	double totalInvested = 0.0;
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
				cash += monthlyCapital;
				totalInvested += monthlyCapital;

				PriceHistory::ReverseIterator rit(current);
				int stepped = 0;
				while (stepped < lookbackDays && rit != history->rend()) {
					++rit;
					stepped++;
				}
				if (stepped == lookbackDays && rit != history->rend()) {
					double price_6mo_ago = (*rit).close;
					double currentPrice = current->close;
					double trailingReturn = (currentPrice - price_6mo_ago) / price_6mo_ago * 100.0;
					if (trailingReturn > momentumThreshold && cash > 0) {
						shares += cash / currentPrice;
						cash = 0;
						result.totalTrades++;
					}
					else if (trailingReturn < 0 && shares > 0) {
						cash += shares * currentPrice;
						shares = 0;
						result.totalTrades++;
					}
				}
				double portfolioValue = shares * current->close + cash;
				result.finalValue = portfolioValue;
				updateMaxDrawdown(portfolioValue, peakValue, maxDrawdown);
				hasPortfolioValue = true;
				break;
			}
			current = current->next;
		}
	}
}

	result.totalInvested = totalInvested;
	if (hasPortfolioValue) {
		result.maxDrawdown = maxDrawdown;
		if (result.totalInvested > 0.0) {
			result.totalReturn = (result.finalValue - result.totalInvested) / result.totalInvested * 100.0;
			result.cagr = calculateCAGR(result.totalInvested, result.finalValue, endYear - startYear);
		}
	}
	return result;
}

string MomentumStrategy::getName() const { 
	return "6-Month Momentum";
}

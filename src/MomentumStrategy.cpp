#include "MomentumStrategy.h"
using namespace std;

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
	double cash = monthlyCapital;
	double totalInvested = monthlyCapital;
	vector<double> portfolioValues;

	for (int year = startYear; year <= endYear; year++) {
	for (int month = 1; month <= 12; month++) {
		PriceNode* current = history->getHead();
		while (current != nullptr) {
			int nodeYear = stoi(current->date.substr(0, 4));
			int nodeMonth = stoi(current->date.substr(5, 2));
			if (nodeYear == year && nodeMonth == month) {
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
				portfolioValues.push_back(shares * current->close + cash);
				break;
			}
			current = current->next;
		}
	}
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

string MomentumStrategy::getName() const { 
	return "6-Month Momentum";
}

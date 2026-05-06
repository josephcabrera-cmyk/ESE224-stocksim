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

	int totalMonths = (endYear - startYear) * 12; 
	double totalBudget = totalMonths * monthlyCapital;
	double remainingBudget = totalBudget;

	double totalInvested = 0.0;
	double shares = 0.0;

	vector<double> portfolioValues;
	vector<double> recentPrices;

	int monthsCompleted = 0;

	for (int year = startYear; year < endYear; year++) {

		for (int month = 1; month <= 12; month++) {

			if (monthsCompleted >= totalMonths) {
				break;
			}

			PriceNode* current = history->getHead();

			while (current != nullptr) {
				int nodeYear = stoi(current->date.substr(0, 4));
				int nodeMonth = stoi(current->date.substr(5, 2));

				if (nodeYear == year && nodeMonth == month) {
					double closePrice = current->close;

					int remainingMonths = totalMonths - monthsCompleted;
					double baseSpend = remainingBudget / remainingMonths;
					double spend = baseSpend;

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

						double dipFromHigh = 0.0;
						double rallyFromLow = 0.0;

						if (high > 0) {
							dipFromHigh = (high - closePrice) / high * 100.0;
						}

						if (low > 0) {
							rallyFromLow = (closePrice - low) / low * 100.0;
						}

						if (dipFromHigh >= dipThreshold) {
							spend = multiplier * baseSpend;
						}
						else if (rallyFromLow >= rallyThreshold) {
							spend = 0.5 * baseSpend;
						}
					}

					double minSpend = 0.25 * baseSpend;
					double maxSpend = 3.0 * baseSpend;

					if (spend < minSpend) {
						spend = minSpend;
					}

					if (spend > maxSpend) {
						spend = maxSpend;
					}

					if (spend > remainingBudget) {
						spend = remainingBudget;
					}

					if (monthsCompleted == totalMonths - 1) {
						spend = remainingBudget;
					}

					if (spend > 0) {
						double sharesBought = spend / closePrice;
						shares += sharesBought;
						totalInvested += spend;
						remainingBudget -= spend;

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

					monthsCompleted++;

					break;
				}

				current = current->next;
			}
		}
	}

	if (!portfolioValues.empty() && result.totalInvested > 0) {
		result.maxDrawdown = calculateMaxDrawdown(portfolioValues);
		result.totalReturn = (result.finalValue - result.totalInvested) / result.totalInvested * 100.0;
		result.cagr = calculateCAGR(portfolioValues.front(), portfolioValues.back(), endYear - startYear);
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

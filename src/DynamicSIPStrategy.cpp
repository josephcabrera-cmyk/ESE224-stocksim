#include "DynamicSIPStrategy.h"
#include <sstream>
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

	double peakValue = 0.0;
	double maxDrawdown = 0.0;
	bool hasPortfolioValue = false;

	const int RECENT_PRICE_LIMIT = 12;
	double recentPrices[RECENT_PRICE_LIMIT];
	int recentStart = 0;
	int recentCount = 0;

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

					double baseSpend = monthlyCapital;
					double spend = baseSpend;

					if (recentCount >= RECENT_PRICE_LIMIT) {
						double high = recentPrices[recentStart];
						double low = recentPrices[recentStart];

						for (int i = 1; i < recentCount; i++) {
							int index = (recentStart + i) % RECENT_PRICE_LIMIT;

							if (recentPrices[index] > high) {
								high = recentPrices[index];
							}

							if (recentPrices[index] < low) {
								low = recentPrices[index];
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
						updateMaxDrawdown(portfolioValue, peakValue, maxDrawdown);
						hasPortfolioValue = true;
					}

					if (recentCount < RECENT_PRICE_LIMIT) {
						int index = (recentStart + recentCount) % RECENT_PRICE_LIMIT;
						recentPrices[index] = closePrice;
						recentCount++;
					}
					else {
						recentPrices[recentStart] = closePrice;
						recentStart = (recentStart + 1) % RECENT_PRICE_LIMIT;
					}

					monthsCompleted++;

					break;
				}

				current = current->next;
			}
		}
	}

	if (hasPortfolioValue && result.totalInvested > 0) {
		result.maxDrawdown = maxDrawdown;
		result.totalReturn = (result.finalValue - result.totalInvested) / result.totalInvested * 100.0;
		result.cagr = calculateCAGR(totalInvested, result.finalValue, endYear - startYear);
	}

	return result;
}
string DynamicSIPStrategy::getName() const { 
	ostringstream out;
	out << "Dynamic SIP (dip=" << dipThreshold
	    << "%, rally=" << rallyThreshold
	    << "%, mult=" << multiplier << "x)";
	return out.str();
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

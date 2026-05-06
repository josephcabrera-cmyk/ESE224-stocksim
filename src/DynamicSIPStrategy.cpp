#include "DynamicSIPStrategy.h"
#include <sstream>
#include <iomanip>
using namespace std;

DynamicSIPStrategy::DynamicSIPStrategy(double dipThreshold,
                                       double rallyThreshold,
                                       double multiplier)
    : dipThreshold(dipThreshold),
      rallyThreshold(rallyThreshold),
      multiplier(multiplier) {}

static int getYear(const string& date) {
    return stoi(date.substr(0, 4));
}

static int getMonth(const string& date) {
    return stoi(date.substr(5, 2));
}

SimResult DynamicSIPStrategy::backtest(PriceHistory* history,
double monthlyCapital, int startYear, int endYear) {
    SimResult result;

    double shares = 0.0;
    double cashReserve = 0.0;
    double totalInvested = 0.0;
    double peakValue = 0.0;
    double maxDrawdown = 0.0;

    int tradeCount = 0;
    int monthCount = 0;

    int lastMonth = -1;
    int lastYear = -1;

    PriceNode* curr = history->getHead();

    while (curr != nullptr) {
        int year = getYear(curr->date);
        int month = getMonth(curr->date);

        if (year >= startYear && year <= endYear) {

            // only invest once per month
            if (year != lastYear || month != lastMonth) {
                monthCount++;

                double high12 = curr->high;
                double low12 = curr->low;

                // look backward up to about 252 trading days
                PriceNode* temp = curr;
                int days = 0;

                while (temp != nullptr && days < 252) {
                    if (temp->high > high12)
                        high12 = temp->high;

                    if (temp->low < low12)
                        low12 = temp->low;

                    temp = temp->prev;
                    days++;
                }

                double price = curr->close;
                double monthlyBudget = monthlyCapital + cashReserve;
                double investAmount = monthlyCapital;

                double dropFromHigh = ((high12 - price) / high12) * 100.0;
                double riseFromLow = ((price - low12) / low12) * 100.0;

                if (dropFromHigh >= dipThreshold) {
                    investAmount = monthlyCapital * multiplier;
                }
                else if (riseFromLow >= rallyThreshold) {
                    investAmount = monthlyCapital * 0.5;
                }

                // cannot invest more than available budget
                if (investAmount > monthlyBudget) {
                    investAmount = monthlyBudget;
                }

                cashReserve = monthlyBudget - investAmount;

                shares += investAmount / price;
                totalInvested += investAmount;
                tradeCount++;

                double portfolioValue = shares * price + cashReserve;

                if (portfolioValue > peakValue) {
                    peakValue = portfolioValue;
                }

                if (peakValue > 0) {
                    double drawdown = (peakValue - portfolioValue) / peakValue;
                    if (drawdown > maxDrawdown) {
                        maxDrawdown = drawdown;
                    }
                }

                lastYear = year;
                lastMonth = month;
            }
        }

        curr = curr->next;
    }

    double finalPrice = 0.0;

PriceNode* finalNode = history->getTail();
while (finalNode != nullptr) {
    int year = getYear(finalNode->date);

    if (year >= startYear && year <= endYear) {
        finalPrice = finalNode->close;
        break;
    }

    finalNode = finalNode->prev;
}

double finalValue = shares * finalPrice + cashReserve;

result.strategyName = getName();
result.finalValue = finalValue;
result.totalInvested = monthCount * monthlyCapital;
result.totalTrades = tradeCount;
result.maxDrawdown = maxDrawdown * 100.0;

if (result.totalInvested > 0.0) {
    result.totalReturn =
        (result.finalValue - result.totalInvested) / result.totalInvested * 100.0;
}

result.cagr = calculateCAGR(result.totalInvested,
                            result.finalValue,
                            endYear - startYear + 1);

return result;
}

string DynamicSIPStrategy::getName() const {
    stringstream ss;

    ss << fixed << setprecision(1);
    ss << "Dynamic SIP "
       << "(Dip " << dipThreshold << "%, "
       << "Rally " << rallyThreshold << "%, "
       << "Multiplier " << multiplier << "x)";

    return ss.str();
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
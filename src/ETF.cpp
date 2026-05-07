#include "ETF.h"
#include "CSVParser.h"
#include <iostream>

static double integerPower(double base, int exponent) {
    double result = 1.0;

    for (int i = 0; i < exponent; i++) {
        result *= base;
    }

    return result;
}

static double nthRoot(double value, int n) {
    if (value <= 0.0 || n <= 0) {
        return 0.0;
    }

    double guess = (value >= 1.0) ? value : 1.0;

    for (int i = 0; i < 40; i++) {
        double denominator = integerPower(guess, n - 1);

        if (denominator == 0.0) {
            return 0.0;
        }

        guess = ((n - 1) * guess + value / denominator) / n;
    }

    return guess;
}

// Constructor
ETF::ETF(const string& ticker, const string& name, const string& sector, double expenseRatio) 
    : Stock(ticker, name, sector), expenseRatio(expenseRatio) {}

ETF::~ETF() {}

double ETF::getExpenseRatio() const {
    return expenseRatio;
}

void ETF::setExpenseRatio(double ratio) {
    expenseRatio = ratio;
}

double ETF::calculate10YearCAGR() const {   

    if (history == nullptr || history->getSize() == 0) {
        return 0.0;
    }

    double endValue = history->getTail()->close;
    int endYear = CSVParser::extractYear(history->getTail()->date);
    if (endYear - CSVParser::extractYear(history->getHead()->date) < 10) {
        return 0.0;
    }

    double startValue = 0;
    int startYear = endYear - 10;

    PriceNode* node = history->getHead();
    while (node != nullptr) {
        if (CSVParser::extractYear(node->date) >= startYear) {
            startValue = node->close;
            break;
        }
        node = node->next;
    }

    if (startValue == 0.0) {
        return 0.0;
    }

    return (nthRoot(endValue / startValue, 10) - 1.0) * 100;

}

void ETF::printSummary() const {
    Stock::printSummary();
    cout << "Expense Ratio: " << expenseRatio * 100 << "%" << endl;
    cout << "10-Year CAGR : " << calculate10YearCAGR() << "%" << endl;
}

string ETF::getType() const {
    return "ETF";
}

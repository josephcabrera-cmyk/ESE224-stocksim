#include "ETF.h"
#include "CSVParser.h"
#include <iostream>
#include <cmath>

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

    return (pow(endValue / startValue, 1.0 / 10.0) - 1.0) * 100;

}

void ETF::printSummary() const {
    Stock::printSummary();
    cout << "Expense Ratio: " << expenseRatio * 100 << "%" << endl;
    cout << "10-Year CAGR : " << calculate10YearCAGR() << "%" << endl;
}

string ETF::getType() const {
    return "ETF";
}
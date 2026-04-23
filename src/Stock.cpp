#include "Stock.h"
#include "CSVParser.h"
#include <iostream>

// Constructor
Stock::Stock(const string& ticker, const string& name, const string& sector)
    : FinancialAsset(ticker, name, sector), history(nullptr) {}

// Destructor
Stock::~Stock() {
    delete history;
}

bool Stock::loadFromCSV(const string& filename) {
    history = CSVParser::loadHistory(filename);
    if (history == nullptr) {
        return false;
    }
    else {
        cout << history->getSize() << " trading days loaded." << endl;
        return true;
    }
}

PriceHistory* Stock::getHistory() const {
    return history;
}

double Stock::getPriceOnDate(const string& date) const {

}

double Stock::getYearStartPrice(int year) const {


}

double Stock::getYearEndPrice(int year) const {


}

double Stock::calculateAnnualReturn(int year) const {


}

void Stock::printSummary() const {


}

string Stock::getType() const {

    
}
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
    if (history == nullptr) {
        return -1.0;
    }

    PriceNode* node = history->findByDate(date);

    if (node == nullptr) {
        return -1.0;
    }   // Returns -1.0 if the date is not found in the history.
    else {
        return node->close;
    }
}

double Stock::getYearStartPrice(int year) const {
    if (history == nullptr) {
        return -1.0;
    }
    
    PriceNode* node = history->getHead();

    while (node != nullptr) {
        if (CSVParser::extractYear(node->date) == year) {
            return node->close;
        }
        node = node->next;
    }
    return -1.0; // Returns -1.0 if no data exists for that year.
}

double Stock::getYearEndPrice(int year) const {
    if (history == nullptr) {
        return -1.0;
    }

    PriceNode* node = history->getTail();

    while (node != nullptr) {
        if (CSVParser::extractYear(node->date) == year) {
            return node->close;
        }
        node = node->prev;
    }
    return -1.0; // Returns -1.0 if no data exists for that year.
}

double Stock::calculateAnnualReturn(int year) const {
    double yearStartPrice = getYearStartPrice(year);
    double yearEndPrice = getYearEndPrice(year);
    if (yearStartPrice == -1.0 || yearEndPrice == -1.0) {
        return 0.0;
    }   // Returns 0.0 if data is unavailable for the given year.
    
    if (yearStartPrice == 0.0) {
        return 0.0;
    }   // cannot divide by zero

    return (yearEndPrice - yearStartPrice) / yearStartPrice * 100;
}

void Stock::printSummary() const {
    if (history == nullptr || history->getSize() == 0) {
        cout << "no data" << endl;
        return;
    }

    cout << "ticker: " << ticker << endl;
    cout << "full name: " << name << endl;
    cout << "sector: " << sector << endl;
    cout << "date range loaded: " << history->getHead()->date << " to " 
        << history->getTail()->date << endl;
    cout << "total trading days: " << history->getSize() << endl;
    cout << "overall return (first to last price): " << (history->getTail()->close
        - history->getHead()->close) / history->getHead()->close * 100;
}

string Stock::getType() const {
    return "Stock";
}
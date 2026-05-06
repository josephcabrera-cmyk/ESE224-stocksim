#include "Portfolio.h"
#include <iostream>
#include <algorithm>

Portfolio::Portfolio(const string& ownerName, double initialCash) 
    : cashBalance(initialCash), ownerName(ownerName) {}

// --- Trading operations ---

// Buy 'shares' of 'ticker' at 'price' on 'date'.
// Deducts from cashBalance. Updates or creates a Position.
// Pushes a TradeRecord onto tradeHistory.
void Portfolio::buyShares(const string& ticker, int shares, double price, const string& date) {
    double totalCost = shares * price;
    
    cashBalance -= totalCost;

    bool found = false;
    for (size_t i = 0; i < holdings.size(); i++) {
        if (holdings[i].ticker == ticker) {
            int newShares = holdings[i].shares + shares;
            double newAvgCost = (holdings[i].shares * holdings[i].avgCostBasis + shares * price) / newShares;
            holdings[i].shares = newShares;
            holdings[i].avgCostBasis = newAvgCost;
            found = true;
            break;
        }
    }

    if (found == false) {
            Position newPos = {ticker, shares, price, price};
            holdings.push_back(newPos);
        }

    TradeRecord record {ticker, date, price, shares, "BUY", totalCost};

    tradeHistory.push(record);
}

// Sell 'shares' of 'ticker' at 'price' on 'date'.
// Adds to cashBalance. Reduces position (remove if shares reach 0).
// Pushes a TradeRecord onto tradeHistory. Does nothing if position not found.
void Portfolio::sellShares(const string& ticker, int shares, double price, const string& date) {
    double totalCost = shares * price;

    bool found = false;
    for (size_t i = 0; i < holdings.size(); i++) {
        if (holdings[i].ticker == ticker) {
            if (holdings[i].shares < shares) {
                return;
            }
            int newShares = holdings[i].shares - shares;
            holdings[i].shares = newShares;
            found = true;
            if (holdings[i].shares == 0) {
                holdings.erase(holdings.begin() + i);
            }
            break;
        }
    }

    if (found == false) {
        return; // Does nothing if position not found.
    }

    cashBalance += totalCost;

    TradeRecord record {ticker, date, price, shares, "SELL", totalCost};

    tradeHistory.push(record);
}

// Reverses the most recent trade by popping the TradeStack and
// performing the opposite transaction (buy→sell, sell→buy).
// Restores cashBalance to its pre-trade state.
void Portfolio::undoLastTrade() {
    if (tradeHistory.isEmpty()) {
        return;
    }

    TradeRecord record = tradeHistory.pop();

    if (record.action == "BUY") {
        cashBalance += record.totalCost;
        for (size_t i = 0; i < holdings.size(); i++) {
            if (holdings[i].ticker == record.ticker) {
                holdings[i].shares -= record.shares;
                if (holdings[i].shares == 0) {
                    holdings.erase(holdings.begin() + i);
                }
                break;
            }
        }
    }
        
    if (record.action == "SELL") {
        cashBalance -= record.totalCost;
        bool found = false;
        for (size_t i = 0; i < holdings.size(); i++) {
            if (holdings[i].ticker == record.ticker) {
                holdings[i].shares += record.shares;
                found = true;
                break;
            }
        }
        if (found == false) {
            Position newPos = {record.ticker, record.shares, record.price, record.price};
            holdings.push_back(newPos);
        }
    }
}

// Add a pending order to the OrderQueue.
void Portfolio::queueOrder(const Order& order)  {
    pendingOrders.enqueue(order);
}

// Dequeue the front Order and execute it if conditions are met:
//   MARKET order → execute immediately at currentPrice
//   LIMIT BUY    → execute if currentPrice <= order.targetPrice
//   LIMIT SELL   → execute if currentPrice >= order.targetPrice
// Prints "Order skipped" if LIMIT conditions are not met (order is discarded).
void Portfolio::executeNextOrder(double currentPrice, const string& date) {
    if (pendingOrders.isEmpty()) {
        return;
    }

    Order front = pendingOrders.dequeue();

    if (front.type == "MARKET") {
        if (front.side == "BUY") {
            buyShares(front.ticker, front.shares, currentPrice, date);
        }
        else {
            sellShares(front.ticker, front.shares, currentPrice, date);
        }
        return;
    }

    else if (front.type == "LIMIT") {

        if (front.side == "BUY") {
            if (currentPrice <= front.targetPrice) {
                buyShares(front.ticker, front.shares, currentPrice, date);
                return;
            }
            else {
                cout << "Order skipped" << endl;
            }
        }

        else {  // SELL
            if (currentPrice >= front.targetPrice) {
                sellShares(front.ticker, front.shares, currentPrice, date);
            }
            else {
                cout << "Order skipped" << endl;
            }
        }

    }
}

// --- Portfolio queries ---

// Returns the total market value of all positions (sum of shares * currentPrice).
double Portfolio::getTotalMarketValue() const {
    double total = 0.0;

    for (size_t i = 0; i < holdings.size(); i++) {
        total += holdings[i].shares * holdings[i].currentPrice;
    }

    return total;
}

// Returns total portfolio value: getTotalMarketValue() + cashBalance.
double Portfolio::getTotalValue() const {
    return getTotalMarketValue() + cashBalance;
}

// Returns total unrealized return across all positions as a percentage:
//   (totalMarketValue - totalCostBasis) / totalCostBasis * 100
double Portfolio::getTotalUnrealizedReturn() const {
    double totalCostBasis = 0.0;

    for (size_t i = 0; i < holdings.size(); i++) {
        totalCostBasis += holdings[i].shares * holdings[i].avgCostBasis;
    }

    if (totalCostBasis == 0.0) {
        return 0.0;
    }

    return (getTotalMarketValue() - totalCostBasis) / totalCostBasis * 100;
}

double Portfolio::getCashBalance() const {
    return cashBalance;
}

// Updates the currentPrice field of a position (called during backtesting).
void Portfolio::updatePrice(const string& ticker, double newPrice) {
    for (size_t i = 0; i < holdings.size(); i++) {
        if (ticker == holdings[i].ticker) {
            holdings[i].currentPrice = newPrice;
            break;
        }
    }
}

// --- Sorting (uses std::sort with custom comparators) ---
void Portfolio::sortHoldingsByUnrealizedReturn() {
    sort(holdings.begin(), holdings.end(), [](const Position& a, const Position& b) { 
            double a_temp = (a.currentPrice - a.avgCostBasis) / a.avgCostBasis;
            double b_temp = (b.currentPrice - b.avgCostBasis) / b.avgCostBasis;
            return a_temp > b_temp;
    });
}  // descending: best performer first

void Portfolio::sortHoldingsByTicker() {
    sort(holdings.begin(), holdings.end(), [](const Position& a, const Position& b) { 
            return a.ticker < b.ticker;
    });
}            // alphabetical ascending

// --- Display ---
void Portfolio::printHoldings() const {
    for (size_t i = 0; i < holdings.size(); i++) {
        cout << holdings[i].ticker << ", " << holdings[i].shares << ", " 
            << holdings[i].avgCostBasis << ", " << holdings[i].currentPrice << endl;
    }
}

void Portfolio::printTradeHistory() const {
    tradeHistory.printAll();
}   // delegates to tradeHistory.printAll()

void Portfolio::printPendingOrders() const {
    pendingOrders.printAll();
}  // delegates to pendingOrders.printAll()
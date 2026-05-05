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
    for (int i = 0; i < holdings.size(); i++) {
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
    for (int i = 0; i < holdings.size(); i++) {
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
        for (int i = 0; i < holdings.size(); i++) {
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
        for (int i = 0; i < holdings.size(); i++) {
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


}

// Dequeue the front Order and execute it if conditions are met:
//   MARKET order → execute immediately at currentPrice
//   LIMIT BUY    → execute if currentPrice <= order.targetPrice
//   LIMIT SELL   → execute if currentPrice >= order.targetPrice
// Prints "Order skipped" if LIMIT conditions are not met (order is discarded).
void Portfolio::executeNextOrder(double currentPrice, const string& date) {


}

// --- Portfolio queries ---

// Returns the total market value of all positions (sum of shares * currentPrice).
double Portfolio::getTotalMarketValue() const {


}

// Returns total portfolio value: getTotalMarketValue() + cashBalance.
double Portfolio::getTotalValue() const {


}

// Returns total unrealized return across all positions as a percentage:
//   (totalMarketValue - totalCostBasis) / totalCostBasis * 100
double Portfolio::getTotalUnrealizedReturn() const {

}

double Portfolio::getCashBalance() const {


}

// Updates the currentPrice field of a position (called during backtesting).
void Portfolio::updatePrice(const string& ticker, double newPrice) {


}

// --- Sorting (uses std::sort with custom comparators) ---
void Portfolio::sortHoldingsByUnrealizedReturn() {


}  // descending: best performer first

void Portfolio::sortHoldingsByTicker() {


}            // alphabetical ascending

// --- Display ---
void Portfolio::printHoldings() const {


}

void Portfolio::printTradeHistory() const {


}   // delegates to tradeHistory.printAll()

void Portfolio::printPendingOrders() const {


}  // delegates to pendingOrders.printAll()
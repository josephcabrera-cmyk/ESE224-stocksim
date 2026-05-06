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
        cout << "No pending orders." << endl;
        return;
    }

    Order order = pendingOrders.dequeue();

    bool shouldExecute = false;
    if (order.type == "MARKET") {
        shouldExecute = true;
    } else if (order.type == "LIMIT") {
        if (order.side == "BUY" && currentPrice <= order.targetPrice) {
            shouldExecute = true;
        } else if (order.side == "SELL" && currentPrice >= order.targetPrice) {
            shouldExecute = true;
        }
    }

    if (!shouldExecute) {
        cout << "Order skipped" << endl;
        return;
    }

    if (order.side == "BUY") {
        buyShares(order.ticker, order.shares, currentPrice, date);
    } else if (order.side == "SELL") {
        sellShares(order.ticker, order.shares, currentPrice, date);
    }
}

// --- Portfolio queries ---

// Returns the total market value of all positions (sum of shares * currentPrice).
double Portfolio::getTotalMarketValue() const {
    double totalMarketValue = 0.0;

    for (size_t i = 0; i < holdings.size(); i++) {
        totalMarketValue += holdings[i].shares * holdings[i].currentPrice;
    }

    return totalMarketValue;
}

// Returns total portfolio value: getTotalMarketValue() + cashBalance.
double Portfolio::getTotalValue() const {
    return getTotalMarketValue() + cashBalance;
}

// Returns total unrealized return across all positions as a percentage:
//   (totalMarketValue - totalCostBasis) / totalCostBasis * 100
double Portfolio::getTotalUnrealizedReturn() const {
    double totalMarketValue = 0.0;
    double totalCostBasis = 0.0;

    for (size_t i = 0; i < holdings.size(); i++) {
        totalMarketValue += holdings[i].shares * holdings[i].currentPrice;
        totalCostBasis += holdings[i].shares * holdings[i].avgCostBasis;
    }

    if (totalCostBasis == 0.0) {
        return 0.0;
    }

    return ((totalMarketValue - totalCostBasis) / totalCostBasis) * 100.0;

}

double Portfolio::getCashBalance() const {
    return cashBalance;
}

// Updates the currentPrice field of a position (called during backtesting).
void Portfolio::updatePrice(const string& ticker, double newPrice) {
    for (size_t i = 0; i < holdings.size(); i++) {
        if (holdings[i].ticker == ticker) {
            holdings[i].currentPrice = newPrice;
            return;
        }
    }

}

// --- Sorting (uses std::sort with custom comparators) ---
void Portfolio::sortHoldingsByUnrealizedReturn() {
    sort(holdings.begin(), holdings.end(), [](const Position& left, const Position& right) {
        double leftReturn = 0.0;
        double rightReturn = 0.0;

        if (left.avgCostBasis != 0.0) {
            leftReturn = ((left.currentPrice - left.avgCostBasis) / left.avgCostBasis) * 100.0;
        }
        if (right.avgCostBasis != 0.0) {
            rightReturn = ((right.currentPrice - right.avgCostBasis) / right.avgCostBasis) * 100.0;
        }

        return leftReturn > rightReturn;
    });

}  // descending: best performer first

void Portfolio::sortHoldingsByTicker() {
    sort(holdings.begin(), holdings.end(), [](const Position& left, const Position& right) {
        return left.ticker < right.ticker;
    });

}            // alphabetical ascending

// --- Display ---
void Portfolio::printHoldings() const {
    cout << "Portfolio for " << ownerName << endl;
    cout << "Cash Balance: " << cashBalance << endl;
    cout << "Holdings:" << endl;

    if (holdings.empty()) {
        cout << "  (none)" << endl;
        return;
    }

    for (size_t i = 0; i < holdings.size(); i++) {
        cout << "  Ticker: " << holdings[i].ticker
             << " | Shares: " << holdings[i].shares
             << " | Avg Cost: " << holdings[i].avgCostBasis
             << " | Current Price: " << holdings[i].currentPrice
             << endl;
    }

}

void Portfolio::printTradeHistory() const {
    tradeHistory.printAll();
}   // delegates to tradeHistory.printAll()

void Portfolio::printPendingOrders() const {
    pendingOrders.printAll();
}  // delegates to pendingOrders.printAll()
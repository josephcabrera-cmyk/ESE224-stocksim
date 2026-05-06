/*
 * ESE 224 – Final Project: StockSim
 * Historical Market Analyzer & Trading Strategy Simulator
 *
 * Student Name : ___________________________
 * Student ID   : ___________________________
 *
 * Instructions:
 *   1. Implement all classes listed in the header files under include/.
 *   2. Create corresponding .cpp files in src/ for each header.
 *   3. Place your Yahoo Finance CSV files in data/ (SPY.csv, AAPL.csv, TSLA.csv).
 *   4. Complete the menu handlers below — each case should call the relevant
 *      class methods you implemented.
 *   5. Do NOT use std::queue, std::stack, std::list, std::map, std::unordered_map,
 *      or any external library. std::vector, std::string, std::sort are allowed.
 *
 * Compile with C++11 or later:
 *   g++ -std=c++11 -Iinclude src/*.cpp main.cpp -o stocksim
 */

#include <iostream>
#include <string>
#include <vector>

#include "Stock.h"
#include "ETF.h"
#include "PriceHistory.h"
#include "StockBST.h"
#include "Portfolio.h"

#include "FixedSIPStrategy.h"
#include "DynamicSIPStrategy.h"
#include "GoldenCrossStrategy.h"
#include "MomentumStrategy.h"

using namespace std;

// Helper function to select which loaded asset to use
PriceHistory* chooseHistory(Stock& spx, Stock& amzn, Stock& nvda) {
    int choice;

    cout << "\nChoose dataset:" << endl;
    cout << "[1] SPX" << endl;
    cout << "[2] AMZN" << endl;
    cout << "[3] NVDA" << endl;
    cout << "Enter choice: ";
    cin >> choice;

    if (choice == 1) {
        return spx.getHistory();
    } else if (choice == 2) {
        return amzn.getHistory();
    } else if (choice == 3) {
        return nvda.getHistory();
    }

    cout << "Invalid choice. Using SPX by default." << endl;
    return spx.getHistory();
}

string chooseTicker() {
    int choice;

    cout << "\nChoose ticker:" << endl;
    cout << "[1] SPX" << endl;
    cout << "[2] AMZN" << endl;
    cout << "[3] NVDA" << endl;
    cout << "Enter choice: ";
    cin >> choice;

    if (choice == 1) return "SPX";
    if (choice == 2) return "AMZN";
    if (choice == 3) return "NVDA";

    return "SPX";
}

Stock* chooseStock(Stock& spx, Stock& amzn, Stock& nvda) {
    int choice;

    cout << "\nChoose stock:" << endl;
    cout << "[1] SPX" << endl;
    cout << "[2] AMZN" << endl;
    cout << "[3] NVDA" << endl;
    cout << "Enter choice: ";
    cin >> choice;

    if (choice == 1) return &spx;
    if (choice == 2) return &amzn;
    if (choice == 3) return &nvda;

    cout << "Invalid choice. Using SPX by default." << endl;
    return &spx;
}

double getLatestPrice(Stock* stock) {
    if (stock == nullptr || stock->getHistory() == nullptr || stock->getHistory()->getTail() == nullptr) {
        return 0.0;
    }

    return stock->getHistory()->getTail()->close;
}

string getLatestDate(Stock* stock) {
    if (stock == nullptr || stock->getHistory() == nullptr || stock->getHistory()->getTail() == nullptr) {
        return "";
    }

    return stock->getHistory()->getTail()->date;
}

void printStrategyComparison(SimResult r1, SimResult r2, SimResult r3, SimResult r4) {
    cout << "\n===== Strategy Comparison =====" << endl;

    cout << "Strategy: " << r1.strategyName << endl;
    cout << "Final Value: $" << r1.finalValue << endl;
    cout << "Total Invested: $" << r1.totalInvested << endl;
    cout << "Total Return: " << r1.totalReturn << "%" << endl;
    cout << "CAGR: " << r1.cagr << "%" << endl;
    cout << "Max Drawdown: " << r1.maxDrawdown << "%" << endl;
    cout << "Trades: " << r1.totalTrades << endl;
    cout << endl;

    cout << "Strategy: " << r2.strategyName << endl;
    cout << "Final Value: $" << r2.finalValue << endl;
    cout << "Total Invested: $" << r2.totalInvested << endl;
    cout << "Total Return: " << r2.totalReturn << "%" << endl;
    cout << "CAGR: " << r2.cagr << "%" << endl;
    cout << "Max Drawdown: " << r2.maxDrawdown << "%" << endl;
    cout << "Trades: " << r2.totalTrades << endl;
    cout << endl;

    cout << "Strategy: " << r3.strategyName << endl;
    cout << "Final Value: $" << r3.finalValue << endl;
    cout << "Total Invested: $" << r3.totalInvested << endl;
    cout << "Total Return: " << r3.totalReturn << "%" << endl;
    cout << "CAGR: " << r3.cagr << "%" << endl;
    cout << "Max Drawdown: " << r3.maxDrawdown << "%" << endl;
    cout << "Trades: " << r3.totalTrades << endl;
    cout << endl;

    cout << "Strategy: " << r4.strategyName << endl;
    cout << "Final Value: $" << r4.finalValue << endl;
    cout << "Total Invested: $" << r4.totalInvested << endl;
    cout << "Total Return: " << r4.totalReturn << "%" << endl;
    cout << "CAGR: " << r4.cagr << "%" << endl;
    cout << "Max Drawdown: " << r4.maxDrawdown << "%" << endl;
    cout << "Trades: " << r4.totalTrades << endl;
}

int main() {
    string studentName;
    string studentID;

    cout << "Enter student name: ";
    getline(cin, studentName);

    cout << "Enter student ID: ";
    getline(cin, studentID);

    Stock spx("SPX", "S&P 500 Index", "Index");
    Stock amzn("AMZN", "Amazon Inc.", "Consumer Discretionary");
    Stock nvda("NVDA", "NVIDIA Corporation", "Technology");

    Portfolio portfolio(studentName, 100000.0);
    StockBST performanceBST;

    bool dataLoaded = false;
    int choice = -1;

    while (choice != 0) {
        cout << "\n===== StockSim: Historical Market Analyzer =====" << endl;
        cout << "Student: " << studentName << " | ID: " << studentID << endl;
        cout << " [1] Load stock data from CSV" << endl;
        cout << " [2] Display price history (linked list traversal)" << endl;
        cout << " [3] Search by date range" << endl;
        cout << " [4] Find stocks by return range (BST range query)" << endl;
        cout << " [5] Insert stock into performance BST" << endl;
        cout << " [6] Display BST (inorder / preorder / postorder)" << endl;
        cout << " [7] Add stock to portfolio" << endl;
        cout << " [8] Remove stock from portfolio" << endl;
        cout << " [9] Queue a pending order" << endl;
        cout << "[10] Execute next pending order" << endl;
        cout << "[11] Undo last trade" << endl;
        cout << "[12] Run strategy simulation (choose strategy)" << endl;
        cout << "[13] Compare all strategies head-to-head" << endl;
        cout << "[14] Display portfolio summary" << endl;
        cout << "[15] Display full trade history" << endl;
        cout << " [0] Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case  1: menuLoadData(etfManager, stockManager);                        break;
            case  2: menuDisplayHistory(stockManager, etfManager);                  break;
            case  3: menuSearchByDate(stockManager, etfManager);                    break;
            case  4: menuBSTRangeSearch(performanceBST);                            break;
            case  5: menuInsertIntoBST(performanceBST, stockManager);               break;
            case  6: menuDisplayBST(performanceBST);                                break;
            case  7: menuAddToPortfolio(portfolio);                                 break;
            case  8: menuRemoveFromPortfolio(portfolio);                            break;
            case  9: menuQueueOrder(portfolio);                                     break;
            case 10: menuExecuteOrder(portfolio);                                   break;
            case 11: menuUndoTrade(portfolio);                                      break;
            case 12: menuRunStrategy(etfManager, stockManager);                     break;
            case 13: menuCompareStrategies(etfManager);                             break;
            case 14: menuPortfolioSummary(portfolio);                               break;
            case 15: menuTradeHistory(portfolio);                                   break;
            case  0: cout << "Goodbye, " << studentName << "!\n";                  break;
            default: cout << "Invalid choice. Please enter 0–15.\n";               break;
        }
    }

    return 0;
}

// ---------------------------------------------------------------
// Menu handler stubs — implement each one below
// ---------------------------------------------------------------

void menuLoadData(StockManager<ETF>& etfManager, StockManager<Stock>& stockManager) {
    // TODO:
    //  Prompt the user to choose which ticker to load (SPY, AAPL, TSLA, or all).
    //  For SPY: create a new ETF("SPY", "SPDR S&P 500 ETF", "Index", 0.0003),
    //           call loadFromCSV("data/SPY.csv"), add to etfManager.
    //  For AAPL/TSLA: create Stock objects, load CSV, add to stockManager.
    cout << "(TODO: implement menuLoadData)" << endl;
}

void menuDisplayHistory(StockManager<Stock>& stockManager, StockManager<ETF>& etfManager) {
    // TODO:
    //  Ask user for a ticker. Find it in stockManager or etfManager.
    //  Ask how many records to display (or "all").
    //  Use the PriceHistory forward Iterator to print records.
    cout << "(TODO: implement menuDisplayHistory)" << endl;
}

void menuSearchByDate(StockManager<Stock>& stockManager, StockManager<ETF>& etfManager) {
    // TODO:
    //  Ask user for ticker, start date, and end date.
    //  Call history->printRange(startDate, endDate).
    cout << "(TODO: implement menuSearchByDate)" << endl;
}

void menuBSTRangeSearch(StockBST& bst) {
    // TODO:
    //  Ask for a low and high return % value.
    //  Call bst.rangeSearch(low, high, results) and print each result.
    cout << "(TODO: implement menuBSTRangeSearch)" << endl;
}

void menuInsertIntoBST(StockBST& bst, StockManager<Stock>& stockManager) {
    // TODO:
    //  Ask for a ticker and a year.
    //  Find the stock, compute calculateAnnualReturn(year), insert into BST.
    cout << "(TODO: implement menuInsertIntoBST)" << endl;
}

void menuDisplayBST(StockBST& bst) {
    // TODO:
    //  Ask user which traversal: inorder / preorder / postorder.
    //  Call the appropriate BST method.
    cout << "(TODO: implement menuDisplayBST)" << endl;
}

void menuAddToPortfolio(Portfolio& portfolio) {
    // TODO:
    //  Prompt for ticker, shares, price, date. Call portfolio.buyShares(...).
    cout << "(TODO: implement menuAddToPortfolio)" << endl;
}

void menuRemoveFromPortfolio(Portfolio& portfolio) {
    // TODO:
    //  Prompt for ticker, shares, current price, date. Call portfolio.sellShares(...).
    cout << "(TODO: implement menuRemoveFromPortfolio)" << endl;
}

void menuQueueOrder(Portfolio& portfolio) {
    // TODO:
    //  Prompt for order details (ticker, type, side, target price, shares, date).
    //  Build an Order struct and call portfolio.queueOrder(order).
    cout << "(TODO: implement menuQueueOrder)" << endl;
}

void menuExecuteOrder(Portfolio& portfolio) {
    // TODO:
    //  Ask for the current market price and today's date.
    //  Call portfolio.executeNextOrder(currentPrice, date).
    cout << "(TODO: implement menuExecuteOrder)" << endl;
}

void menuUndoTrade(Portfolio& portfolio) {
    // TODO: Call portfolio.undoLastTrade() and confirm to the user.
    cout << "(TODO: implement menuUndoTrade)" << endl;
}

void menuRunStrategy(StockManager<ETF>& etfManager, StockManager<Stock>& stockManager) {
    // TODO:
    //  Sub-menu: choose strategy (1=FixedSIP, 2=DynamicSIP, 3=GoldenCross, 4=Momentum).
    //  Ask for: ticker, monthlyCapital, startYear, endYear, strategy parameters.
    //  Find the asset, get its PriceHistory, create the strategy object,
    //  call strategy.backtest(...), then strategy.printResult(result).
    cout << "(TODO: implement menuRunStrategy)" << endl;
}

void menuCompareStrategies(StockManager<ETF>& etfManager) {
    // TODO:
    //  Use SPY (find in etfManager). Ask for monthlyCapital, startYear, endYear.
    //  Run all 4 strategies with default parameters.
    //  Print a comparison table showing all SimResults side by side.
    cout << "(TODO: implement menuCompareStrategies)" << endl;
}

void menuPortfolioSummary(Portfolio& portfolio) {
    portfolio.printHoldings();
}

        else if (choice == 15) {
            portfolio.printTradeHistory();
        }

        else if (choice == 0) {
            cout << "Exiting StockSim." << endl;
        }

        else {
            cout << "Invalid menu choice." << endl;
        }
    }

    return 0;
}
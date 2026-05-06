/*
 * ESE 224 – Final Project: StockSim
 * Historical Market Analyzer & Trading Strategy Simulator
 *
 * Student Names : Joseph Cabrera & Danny Ouyang
 * Student IDs   : 
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

// Include all your headers here once implemented
#include "include/FinancialAsset.h"
#include "include/PriceNode.h"
#include "include/PriceHistory.h"
#include "include/CSVParser.h"
#include "include/Stock.h"
#include "include/ETF.h"
#include "include/CircularQueue.h"
#include "include/TradeStack.h"
#include "include/OrderQueue.h"
#include "include/StockBST.h"
#include "include/Portfolio.h"
#include "include/TradingStrategy.h"
#include "include/FixedSIPStrategy.h"
#include "include/DynamicSIPStrategy.h"
#include "include/GoldenCrossStrategy.h"
#include "include/MomentumStrategy.h"
#include "include/StockManager.h"

using namespace std;

Stock* chooseStock(StockManager<Stock>& sm, StockManager<ETF>& em) {
    int choice;
    cout << "\nChoose stock:\n[1] SPX\n[2] AMZN\n[3] NVDA\nEnter choice: ";
    cin >> choice;
    if (choice == 1) return em.findByTicker("SPX");
    if (choice == 2) return sm.findByTicker("AMZN");
    if (choice == 3) return sm.findByTicker("NVDA");
    cout << "Invalid. Using SPX." << endl;
    return em.findByTicker("SPX");
}

PriceHistory* chooseHistory(StockManager<Stock>& sm, StockManager<ETF>& em) {
    Stock* s = chooseStock(sm, em);
    return (s == nullptr) ? nullptr : s->getHistory();
}

// Get latest close price
double getLatestPrice(Stock* stock) {
    if (stock == nullptr || stock->getHistory() == nullptr ||
        stock->getHistory()->getTail() == nullptr) {
        return 0.0;
    }

    return stock->getHistory()->getTail()->close;
}

// Get latest date
string getLatestDate(Stock* stock) {
    if (stock == nullptr || stock->getHistory() == nullptr ||
        stock->getHistory()->getTail() == nullptr) {
        return "";
    }

    return stock->getHistory()->getTail()->date;
}

// Print comparison results
void printStrategyComparison(const SimResult& r1,
                             const SimResult& r2,
                             const SimResult& r3,
                             const SimResult& r4) {
    cout << "\n===== Strategy Comparison =====" << endl;

    cout << "\nStrategy: " << r1.strategyName << endl;
    cout << "Final Value: $" << r1.finalValue << endl;
    cout << "Total Invested: $" << r1.totalInvested << endl;
    cout << "Total Return: " << r1.totalReturn << "%" << endl;
    cout << "CAGR: " << r1.cagr << "%" << endl;
    cout << "Max Drawdown: " << r1.maxDrawdown << "%" << endl;
    cout << "Trades: " << r1.totalTrades << endl;

    cout << "\nStrategy: " << r2.strategyName << endl;
    cout << "Final Value: $" << r2.finalValue << endl;
    cout << "Total Invested: $" << r2.totalInvested << endl;
    cout << "Total Return: " << r2.totalReturn << "%" << endl;
    cout << "CAGR: " << r2.cagr << "%" << endl;
    cout << "Max Drawdown: " << r2.maxDrawdown << "%" << endl;
    cout << "Trades: " << r2.totalTrades << endl;

    cout << "\nStrategy: " << r3.strategyName << endl;
    cout << "Final Value: $" << r3.finalValue << endl;
    cout << "Total Invested: $" << r3.totalInvested << endl;
    cout << "Total Return: " << r3.totalReturn << "%" << endl;
    cout << "CAGR: " << r3.cagr << "%" << endl;
    cout << "Max Drawdown: " << r3.maxDrawdown << "%" << endl;
    cout << "Trades: " << r3.totalTrades << endl;

    cout << "\nStrategy: " << r4.strategyName << endl;
    cout << "Final Value: $" << r4.finalValue << endl;
    cout << "Total Invested: $" << r4.totalInvested << endl;
    cout << "Total Return: " << r4.totalReturn << "%" << endl;
    cout << "CAGR: " << r4.cagr << "%" << endl;
    cout << "Max Drawdown: " << r4.maxDrawdown << "%" << endl;
    cout << "Trades: " << r4.totalTrades << endl;
}

    void parameterSweep(ETF* spy,
                    double monthlyCapital,
                    int startYear,
                    int endYear,
                    StockBST& bst) {

    cout << "\n===== Dynamic SIP Parameter Sweep =====" << endl;

    for (double dipThreshold = 3.0;
         dipThreshold <= 20.0;
         dipThreshold += 1.0) {

        DynamicSIPStrategy strategy(
            dipThreshold,
            10.0,
            2.0
        );

        SimResult result =
            strategy.backtest(
                spy->getHistory(),
                monthlyCapital,
                startYear,
                endYear
            );

        string label =
            "dip=" +
            to_string((int)dipThreshold) +
            "%";

        bst.insert(
            label,
            result.finalValue,
            0
        );

        cout << label
             << " -> Final Value: $"
             << result.finalValue
             << endl;
    }

    cout << "\n===== Ranked Results =====" << endl;

    bst.inorder();

    StockBST::BSTNode* best = bst.findMax();

    if (best != nullptr) {
        cout << "\n===== BEST THRESHOLD =====" << endl;

        cout << "Strategy: "
             << best->ticker
             << endl;

        cout << "Final Value: $"
             << best->key
             << endl;
    }
}
// ---------------------------------------------------------------
// main
// ---------------------------------------------------------------
int main() {
    // --- Student login ---
    string studentName, studentID;
    cout << "========================================\n";
    cout << "  ESE 224 StockSim — Student Login\n";
    cout << "========================================\n";
    cout << "Enter your full name: ";
    getline(cin, studentName);
    cout << "Enter your student ID: ";
    getline(cin, studentID);
    cout << "\nWelcome, " << studentName << "!\n";

    // --- Initialize shared objects ---
    StockManager<ETF>   etfManager;
    StockManager<Stock> stockManager;
    StockBST            performanceBST;
    Portfolio           portfolio(studentName, 10000.0);  // start with $10,000 cash

    bool dataLoaded = false;
    int choice = -1;

    // ---------------------------------------------------------------
    // Utility: print the main menu
    // ---------------------------------------------------------------
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
        cout << "[16] Run Dynamic SIP Parameter Sweep (BONUS)" << endl;
        cout << " [0] Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            if (dataLoaded) {
                cout << "Data already loaded." << endl;
                continue;
            }
            ETF* spx = new ETF("SPX", "S&P 500 Index", "Index", 0.0003);
            Stock* amzn = new Stock("AMZN", "Amazon Inc.", "Consumer Discretionary");
            Stock* nvda = new Stock("NVDA", "NVIDIA Corporation", "Technology");

            bool spxLoaded = spx->loadFromCSV("data/SPX.csv");
            bool amznLoaded = amzn->loadFromCSV("data/AMZN.csv");
            bool nvdaLoaded = nvda->loadFromCSV("data/NVidia_stock_history.csv");

            if (spxLoaded && amznLoaded && nvdaLoaded) {
                etfManager.addAsset(spx);
                stockManager.addAsset(amzn);
                stockManager.addAsset(nvda);
                dataLoaded = true;
                cout << "All stock data loaded successfully." << endl;
            } else {
                delete spx; delete amzn; delete nvda;
                cout << "One or more CSV files failed to load." << endl;
            }
        }

        else if (choice == 2) {
            if (!dataLoaded) {
                cout << "Load data first." << endl;
                continue;
            }

            Stock* stock = chooseStock(stockManager, etfManager);

            if (stock->getHistory() == nullptr) {
                cout << "No history loaded." << endl;
                continue;
            }

            PriceNode* current = stock->getHistory()->getHead();

            while (current != nullptr) {
                cout << current->date
                     << " | Open: " << current->open
                     << " | High: " << current->high
                     << " | Low: " << current->low
                     << " | Close: " << current->close
                     << " | Volume: " << current->volume
                     << endl;

                current = current->next;
            }
        }

        else if (choice == 3) {
            if (!dataLoaded) {
                cout << "Load data first." << endl;
                continue;
            }

            Stock* stock = chooseStock(stockManager, etfManager);

            string startDate;
            string endDate;

            cout << "Enter start date YYYY-MM-DD: ";
            cin >> startDate;

            cout << "Enter end date YYYY-MM-DD: ";
            cin >> endDate;

            stock->getHistory()->printRange(startDate, endDate);
        }

        else if (choice == 4) {
            if (!dataLoaded) {
                cout << "Load data first." << endl;
                continue;
            }

            double low;
            double high;

            cout << "Enter low return %: ";
            cin >> low;

            cout << "Enter high return %: ";
            cin >> high;

            vector<StockBST::BSTNode*> results;
            performanceBST.rangeSearch(low, high, results);

            cout << "\nResults in range:" << endl;

            if (results.empty()) {
                cout << "No BST entries found in that range." << endl;
            }

            for (size_t i = 0; i < (int)results.size(); i++) {
                cout << results[i]->ticker
                     << " | Year: " << results[i]->year
                     << " | Return: " << results[i]->key << "%"
                     << endl;
            }
        }

        else if (choice == 5) {
            if (!dataLoaded) {
                cout << "Load data first." << endl;
                continue;
            }

            Stock* stock = chooseStock(stockManager, etfManager);

            int year;
            cout << "Enter year to calculate return: ";
            cin >> year;

            double annualReturn = stock->calculateAnnualReturn(year);

            performanceBST.insert(stock->getTicker(), annualReturn, year);

            cout << "Inserted "
                 << stock->getTicker()
                 << " return for "
                 << year
                 << ": "
                 << annualReturn
                 << "%"
                 << endl;
        }

        else if (choice == 6) {
            int traversalChoice;

            cout << "\nBST Traversal:" << endl;
            cout << "[1] Inorder" << endl;
            cout << "[2] Preorder" << endl;
            cout << "[3] Postorder" << endl;
            cout << "Enter choice: ";
            cin >> traversalChoice;

            if (traversalChoice == 1) {
                performanceBST.inorder();
            } else if (traversalChoice == 2) {
                performanceBST.preorder();
            } else if (traversalChoice == 3) {
                performanceBST.postorder();
            } else {
                cout << "Invalid traversal choice." << endl;
            }
        }

        else if (choice == 7) {
            if (!dataLoaded) {
                cout << "Load data first." << endl;
                continue;
            }

            Stock* stock = chooseStock(stockManager, etfManager);

            int shares;
            cout << "Enter shares to buy: ";
            cin >> shares;

            double price = getLatestPrice(stock);
            string date = getLatestDate(stock);

            if (portfolio.getCashBalance() < shares * price) { 
                cout << "Insufficient cash" << endl; continue;
            }

            portfolio.buyShares(stock->getTicker(), shares, price, date);
            portfolio.updatePrice(stock->getTicker(), price);

            cout << "Bought " << shares << " shares of "
                 << stock->getTicker() << " at " << price << endl;
        }

        else if (choice == 8) {
            if (!dataLoaded) {
                cout << "Load data first." << endl;
                continue;
            }

            Stock* stock = chooseStock(stockManager, etfManager);

            int shares;
            cout << "Enter shares to sell/remove: ";
            cin >> shares;

            double price = getLatestPrice(stock);
            string date = getLatestDate(stock);

            bool success = portfolio.sellShares(stock->getTicker(), shares, price, date);

            if (success == true) {

            cout << "Sold " << shares << " shares of "
                 << stock->getTicker() << " at " << price << endl;
            }
            else {
                cout << "Insufficient shares" << endl;
            }
        }

        else if (choice == 9) {
            Order order;

            cout << "Enter ticker: ";
            cin >> order.ticker;

            cout << "Enter type MARKET or LIMIT: ";
            cin >> order.type;

            cout << "Enter side BUY or SELL: ";
            cin >> order.side;

            cout << "Enter target price, use 0 for MARKET: ";
            cin >> order.targetPrice;

            cout << "Enter shares: ";
            cin >> order.shares;

            cout << "Enter submitted date YYYY-MM-DD: ";
            cin >> order.submittedDate;

            portfolio.queueOrder(order);

            cout << "Order queued." << endl;
        }

        else if (choice == 10) {
            if (!dataLoaded) {
                cout << "Load data first." << endl;
                continue;
            }

            Stock* stock = chooseStock(stockManager, etfManager);

            double currentPrice = getLatestPrice(stock);
            string date = getLatestDate(stock);

            portfolio.executeNextOrder(currentPrice, date);
        }

        else if (choice == 11) {
            portfolio.undoLastTrade();
            cout << "Last trade undone." << endl;
        }

        else if (choice == 12) {
            if (!dataLoaded) {
                cout << "Load data first." << endl;
                continue;
            }

            PriceHistory* history = chooseHistory(stockManager, etfManager);

            double monthlyCapital;
            int startYear;
            int endYear;
            int strategyChoice;

            cout << "Enter monthly capital: ";
            cin >> monthlyCapital;

            cout << "Enter start year: ";
            cin >> startYear;

            cout << "Enter end year: ";
            cin >> endYear;

            cout << "\nChoose strategy:" << endl;
            cout << "[1] Fixed SIP" << endl;
            cout << "[2] Dynamic SIP" << endl;
            cout << "[3] Golden Cross" << endl;
            cout << "[4] Momentum" << endl;
            cout << "Enter choice: ";
            cin >> strategyChoice;

            if (strategyChoice == 1) {
                FixedSIPStrategy strategy;
                SimResult result = strategy.backtest(history, monthlyCapital, startYear, endYear);
                strategy.printResult(result);
            }

            else if (strategyChoice == 2) {
                double dipThreshold;
                double rallyThreshold;
                double multiplier;

                cout << "Enter dip threshold percent: ";
                cin >> dipThreshold;

                cout << "Enter rally threshold percent: ";
                cin >> rallyThreshold;

                cout << "Enter multiplier: ";
                cin >> multiplier;

                DynamicSIPStrategy strategy(dipThreshold, rallyThreshold, multiplier);
                SimResult result = strategy.backtest(history, monthlyCapital, startYear, endYear);
                strategy.printResult(result);
            }

            else if (strategyChoice == 3) {
                int shortWindow;
                int longWindow;

                cout << "Enter short MA window, usually 50: ";
                cin >> shortWindow;

                cout << "Enter long MA window, usually 200: ";
                cin >> longWindow;

                GoldenCrossStrategy strategy(shortWindow, longWindow);
                SimResult result = strategy.backtest(history, monthlyCapital, startYear, endYear);
                strategy.printResult(result);
            }

            else if (strategyChoice == 4) {
                double threshold;
                int lookbackDays;

                cout << "Enter momentum threshold percent, example 5: ";
                cin >> threshold;

                cout << "Enter lookback trading days, example 126: ";
                cin >> lookbackDays;

                MomentumStrategy strategy(threshold, lookbackDays);
                SimResult result = strategy.backtest(history, monthlyCapital, startYear, endYear);
                strategy.printResult(result);
            }

            else {
                cout << "Invalid strategy choice." << endl;
            }
        }

        else if (choice == 13) {
            if (!dataLoaded) {
                cout << "Load data first." << endl;
                continue;
            }

            PriceHistory* history = chooseHistory(stockManager, etfManager);

            double monthlyCapital;
            int startYear;
            int endYear;

            cout << "Enter monthly capital: ";
            cin >> monthlyCapital;

            cout << "Enter start year: ";
            cin >> startYear;

            cout << "Enter end year: ";
            cin >> endYear;

            FixedSIPStrategy fixed;
            DynamicSIPStrategy dynamic(10.0, 20.0, 2.0);
            GoldenCrossStrategy golden(50, 200);
            MomentumStrategy momentum(5.0, 126);

            SimResult r1 = fixed.backtest(history, monthlyCapital, startYear, endYear);
            SimResult r2 = dynamic.backtest(history, monthlyCapital, startYear, endYear);
            SimResult r3 = golden.backtest(history, monthlyCapital, startYear, endYear);
            SimResult r4 = momentum.backtest(history, monthlyCapital, startYear, endYear);

            printStrategyComparison(r1, r2, r3, r4);
        }

        else if (choice == 14) {
            portfolio.printHoldings();
        }

        else if (choice == 15) {
            portfolio.printTradeHistory();
        }

         else if (choice == 16) {

            if (!dataLoaded) {
                cout << "Load data first." << endl;
                continue;
            }

            ETF* spy =
                etfManager.findByTicker("SPX");

            if (spy == nullptr) {
                cout << "SPX ETF not found." << endl;
                continue;
            }

            double monthlyCapital;
            int startYear;
            int endYear;

            cout << "Enter monthly capital: ";
            cin >> monthlyCapital;

            cout << "Enter start year: ";
            cin >> startYear;

            cout << "Enter end year: ";
            cin >> endYear;

            StockBST sweepBST;

            parameterSweep(
                spy,
                monthlyCapital,
                startYear,
                endYear,
                sweepBST
            );
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
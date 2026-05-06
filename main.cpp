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

// Choose loaded history
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

// Choose stock object
Stock* chooseStock(Stock& spx, Stock& amzn, Stock& nvda) {
    int choice;

    cout << "\nChoose stock:" << endl;
    cout << "[1] SPX" << endl;
    cout << "[2] AMZN" << endl;
    cout << "[3] NVDA" << endl;
    cout << "Enter choice: ";
    cin >> choice;

    if (choice == 1) {
        return &spx;
    } else if (choice == 2) {
        return &amzn;
    } else if (choice == 3) {
        return &nvda;
    }

    cout << "Invalid choice. Using SPX by default." << endl;
    return &spx;
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

        if (choice == 1) {
            bool spxLoaded = spx.loadFromCSV("data/SPX.csv");
            bool amznLoaded = amzn.loadFromCSV("data/AMZN.csv");
            bool nvdaLoaded = nvda.loadFromCSV("data/NVidia_stock_history.csv");

            if (spxLoaded && amznLoaded && nvdaLoaded) {
                dataLoaded = true;
                cout << "All stock data loaded successfully." << endl;
            } else {
                cout << "One or more CSV files failed to load." << endl;
            }
        }

        else if (choice == 2) {
            if (!dataLoaded) {
                cout << "Load data first." << endl;
                continue;
            }

            Stock* stock = chooseStock(spx, amzn, nvda);

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

            Stock* stock = chooseStock(spx, amzn, nvda);

            string startDate;
            string endDate;

            cout << "Enter start date YYYY-MM-DD: ";
            cin >> startDate;

            cout << "Enter end date YYYY-MM-DD: ";
            cin >> endDate;

            stock->getHistory()->printRange(startDate, endDate);
        }

        else if (choice == 4) {
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

            for (int i = 0; i < (int)results.size(); i++) {
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

            Stock* stock = chooseStock(spx, amzn, nvda);

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

            Stock* stock = chooseStock(spx, amzn, nvda);

            int shares;
            cout << "Enter shares to buy: ";
            cin >> shares;

            double price = getLatestPrice(stock);
            string date = getLatestDate(stock);

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

            Stock* stock = chooseStock(spx, amzn, nvda);

            int shares;
            cout << "Enter shares to sell/remove: ";
            cin >> shares;

            double price = getLatestPrice(stock);
            string date = getLatestDate(stock);

            portfolio.sellShares(stock->getTicker(), shares, price, date);

            cout << "Sold " << shares << " shares of "
                 << stock->getTicker() << " at " << price << endl;
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

            Stock* stock = chooseStock(spx, amzn, nvda);

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

            PriceHistory* history = chooseHistory(spx, amzn, nvda);

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

            PriceHistory* history = chooseHistory(spx, amzn, nvda);

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

        else if (choice == 0) {
            cout << "Exiting StockSim." << endl;
        }

        else {
            cout << "Invalid menu choice." << endl;
        }
    }

    return 0;
}
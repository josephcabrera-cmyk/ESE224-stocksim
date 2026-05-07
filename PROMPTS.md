# COMBINED AI Prompts Log — ESE 224 Final Project

**Student Names:** Joseph Cabrera & Danny Ouyang
**Student IDs:**   114650793 & 115723777

---

## Joseph Cabrera Prompts

### Section: Stock.cpp

**Prompt:** Please implement the constructor and destructor for Stock.cpp

**What you changed / why (optional):**

**Prompt:** How do we print the number of trading days loaded for loadFromCSV.

**Prompt:** why am I getting an error on const override in Stock.cpp?

**Prompt:** the constructor for stock takes ticker, name, and sector. how are we supposed to get price on a date?

**Prompt:** provide me with the skeleton code for getPriceOnDate

**Prompt:** how do we get the specific year data in getYearStartPrice()?

**Prompt:** how do I know the date range loaded for printSummary()?

### Section: ETF.cpp

**Prompt:** why am I getting the error no instance of overloaded function "ETF::ETF" matches the specified type

**Prompt:** how do we get the year in calculate10YearCAGR? How do we know if there is insufficient data?

**Prompt:** why am I getting name followed by '::' must be a class or namespace name on CSVParser::

**Prompt:** do we rewrite everything in Stock printSummary to ETF?

**Prompt:** cmath is actually not on our allowed list of includes. Can we implement without it?

### Section: StockManager.h

**Prompt:** in removeAsset, how do I delete the pointer and erase from vector?

**Prompt:** why am I getting an error on assets[i] == getTicker()

**Prompt:** can you give me the correct conditions for removeAsset

**Prompt:** what is wrong with my syntax on sortByAnnualReturn

### Section: StockBST.cpp

**Prompt:** why am I getting error identifier "BSTNode" is undefined

**Prompt:** Can you give me the pseudocode for insertHelper

**Prompt:** How do we use insertHelper in the wrapper?

**Prompt:** Does StockBST::search just call searchHelper? What does it return?

**Prompt:** What are inorder, preorder, and postorder actually supposed to output?

**Prompt:** What is the logic for clearHelper?

**Prompt:** Can you give me the pseudocode for heightHelper

**Prompt:** Can you clarify what is actually being asked for rangeSearch?

**Prompt:** How do we push into a results vector?

**Prompt:** not sure if include algorithm can actually be used here how do we rewrite without it

### Section: Portfolio.cpp

**Prompt:** Can you give me the pseudocode for buyShares

**Prompt:** Please help me clean up my syntax on buyShares

**Prompt:** Can you check my sellShares logic and make sure it is correct?

**Prompt:** Please help me debug undoLastTrade()

**Prompt:** Am I missing any edge cases on executeNextOrder()?

**Prompt:** How do we calculate sum in getTotalMarketValue()

**Prompt:** What is updatePrice asking? What is backtesting?

**Prompt:** How do we return descending for sortHoldingsByUnrealizedReturn()

### Section: DynamicSIPStrategy.cpp

**Prompt:** What is wrong with my constructor for DynamicSIPStrategy

**Prompt:** Does DynamicSIPStrategy have the same structure as FixedSIPStrategy?

**Prompt:** how do we compare against the 12-month rolling high/low to make this dynamic and not fixed

**Prompt:** How do we find the maximum price over a 12 month period?

**Prompt:** How do I make sure the strategy does not go over-budget?

**Prompt:** Help me debug DynamicSIPStrategy it is acting like FixedSIPStrategy for some reason

**Prompt:** Can we implement this without using vector<double>? Not sure if std vector is allowed here.

### Section: GoldenCrossStrategy.cpp

**Prompt:** How do we use CircularQueue for the moving averages 50 and 200?

**Prompt:** How do we "Enqueue today's close into both ma50Queue and ma200Queue"? What does that mean?

**Prompt:** What are the precise conditions that mean "cross"? i.e. greater than or equal, or just greater than?

**Prompt:** How do I prevent the golden/death cross from being triggered at the inital values?

**Prompt:** Do I properly address all edge cases in GoldenCrossStrategy?

### Section: MomentumStrategy.cpp

**Prompt:** Please give me the basic shell for MomentumStrategy.cpp based on the header including constructor, backtest, getName, etc.

**Prompt:** What does the header mean when it says "ReverseIterator integration is the key requirement here"?

**Prompt:** how do we scan backwards ~126 trading days on PriceHistory, a doubly linked list?

**Prompt:** Is the shape of the code similar to Golden, just with different conditions?

### Section: main.cpp

**Prompt:** How do we build an order struct for menuQueueOrder?

**Prompt:** How do we use StockManager in main to add stocks and ETFs?

**Prompt:** What are some edge cases that we have to fix for our functions in main.cpp?

**Prompt:** SPX seems to be loading pnly 1540 days of data, while the other two load 5031 days. What is the cause of this problem and how do we fix it?

**Prompt:** The CAGR value we are getting from our strategies seems to be far too high. Is this an issue in our main our with our strategy logic?

**Prompt:** Can you please vigorously test the program by testing many different inputs in main.cpp? If you find an issue, please report it and I will classify the issue as needing addressing or not needing addressing.

**Prompt:** Please implement the necessary changes to prevent looping forever and other errors in main

**Prompt:** We are having an issue with case 13 where our output shows a crazy high CAGR when we have a very high final portfolio value. Please help me identify the root cause and solution.

### Section: CSVParser.cpp

**Prompt:** We accidentally used std vector here while not being allowed. How do we implement without std vector?

### Section: FixedSIPStrategy.cpp

**Prompt:** Can we implement without using vector<double>? Not sure if std vector is allowed here based on the guidelines.

### Section: TradingStrategy.cpp

**Prompt:** cmath is used here and I am not sure if that is allowed based on the lab guidelines. Can we implement without?

### Section: bonus_strategy.txt

**Prompt:** Right now our bonus strategy only manipulates dipThreshold, rallyThreshold, and multiplier. Can/should we manipulate other parameters? If so, in what way?

---

## Danny Ouyang Prompts

**Prompt:**
What is CMake on VSCode?

**AI Output:**
- Build automation tool
- Generates build files
- Simplifies compiling

--------------------------------------------------

**Prompt:**
What libraries are used to read files?

**AI Output:**
- fstream
- sstream

--------------------------------------------------

**Prompt:**
What variables should I reference from the CSV?

**AI Output:**
- data

--------------------------------------------------

**Prompt:**
Should I take into consideration different columns in PriceHistory.cpp or CSVParser.cpp?

**AI Output:**
- CSVParser.cpp

--------------------------------------------------

**Prompt:**
How do I catch errors of any type?

**AI Output:**
- catch(...)

--------------------------------------------------

**Prompt:**
What is CMake in VSCode?

**AI Output:**
- Build automation tool
- Generates build files
- Simplifies compiling

--------------------------------------------------

**Prompt:**
How do I parse CSV files?

**AI Output:**
- getline()
- stringstream

--------------------------------------------------

**Prompt:**
How do I skip CSV headers?

**AI Output:**
- getline() once

--------------------------------------------------

**Prompt:**
How do I convert strings to doubles?

**AI Output:**
- stod()

--------------------------------------------------

**Prompt:**
How do I convert strings to integers?

**AI Output:**
- stoi()

--------------------------------------------------

**Prompt:**
How do I validate malformed CSV rows?

**AI Output:**
- check token count

--------------------------------------------------

**Prompt:**
How do I test if a file opened correctly?

**AI Output:**
- file.is_open()

--------------------------------------------------

**Prompt:**
How do I append nodes to a linked list?

**AI Output:**
- tail->next
- update tail

--------------------------------------------------

**Prompt:**
How do I delete linked list nodes?

**AI Output:**
- traverse + delete

--------------------------------------------------

**Prompt:**
How do I search linked lists by date?

**AI Output:**
- while(current)

--------------------------------------------------

**Prompt:**
How do I print linked list ranges?

**AI Output:**
- compare dates

--------------------------------------------------

**Prompt:**
How do I avoid memory leaks?

**AI Output:**
- destructor cleanup

--------------------------------------------------

**Prompt:**
How do I insert into a BST?

**AI Output:**
- recursive insert

--------------------------------------------------

**Prompt:**
How do I traverse BST inorder?

**AI Output:**
- left root right

--------------------------------------------------

**Prompt:**
How do I find max BST node?

**AI Output:**
- go right

--------------------------------------------------

**Prompt:**
How do I range search BST values?

**AI Output:**
- recursive compare

--------------------------------------------------

**Prompt:**
How do I store trade history?

**AI Output:**
- stack or vector

--------------------------------------------------

**Prompt:**
How do I queue pending orders?

**AI Output:**
- circular queue

--------------------------------------------------

**Prompt:**
How do I undo trades?

**AI Output:**
- pop stack

--------------------------------------------------

**Prompt:**
How do I validate enough cash before buying?

**AI Output:**
- compare balance

--------------------------------------------------

**Prompt:**
How do I validate enough shares before selling?

**AI Output:**
- compare holdings

--------------------------------------------------

**Prompt:**
How do I calculate CAGR?

**AI Output:**
- compound growth formula

--------------------------------------------------

**Prompt:**
How do I calculate max drawdown?

**AI Output:**
- peak to trough

--------------------------------------------------

**Prompt:**
How do I compare strategies fairly?

**AI Output:**
- same years + capital

--------------------------------------------------

**Prompt:**
How do I implement Fixed SIP?

**AI Output:**
- fixed monthly investment

--------------------------------------------------

**Prompt:**
How do I implement Dynamic SIP?

**AI Output:**
- react to dips

--------------------------------------------------

**Prompt:**
How do I detect market dips?

**AI Output:**
- compare recent highs

--------------------------------------------------

**Prompt:**
How do I implement Golden Cross?

**AI Output:**
- moving averages

--------------------------------------------------

**Prompt:**
How do I implement momentum investing?

**AI Output:**
- trend strength

--------------------------------------------------

**Prompt:**
How do I automate parameter testing?

**AI Output:**
- parameter sweep loop

--------------------------------------------------

**Prompt:**
How do I compare Dynamic SIP against Fixed SIP?

**AI Output:**
- run both strategies

--------------------------------------------------

**Prompt:**
Why are all parameter sweep outputs identical?

**AI Output:**
- strategy normalization

--------------------------------------------------

**Prompt:**
Why is my linked list crashing?

**AI Output:**
- null pointer issue

--------------------------------------------------

**Prompt:**
Why is my BST traversal unsorted?

**AI Output:**
- incorrect recursion

--------------------------------------------------

**Prompt:**
Why is my CSV parser skipping rows?

**AI Output:**
- getline misuse

--------------------------------------------------

**Prompt:**
Why is my executable locked on Windows?

**AI Output:**
- exe still running

--------------------------------------------------

**Prompt:**
What compile command should I use?

**AI Output:**
- g++ -std=c++11 -Iinclude src/*.cpp main.cpp -o stocksim

--------------------------------------------------

**Prompt:**
How do I run the executable?

**AI Output:**
- ./stocksim

--------------------------------------------------

**Prompt:**
How do I structure SimResult?

**AI Output:**
- metrics container

--------------------------------------------------

**Prompt:**
How do I track portfolio value?

**AI Output:**
- shares * price

--------------------------------------------------

**Prompt:**
How do I update stock prices?

**AI Output:**
- latest close

--------------------------------------------------

**Prompt:**
How do I read dates from CSV?

**AI Output:**
- substr()

--------------------------------------------------

**Prompt:**
How do I compare dates?

**AI Output:**
- YYYY-MM-DD format

--------------------------------------------------

**Prompt:**
How do I safely traverse pointers?

**AI Output:**
- nullptr checks

--------------------------------------------------

**Prompt:**
How do I debug segmentation faults?

**AI Output:**
- trace pointers

--------------------------------------------------

**Prompt:**
How do I test strategies quickly?

**AI Output:**
- shorter date range

--------------------------------------------------

**Prompt:**
How do I print portfolio summary?

**AI Output:**
- holdings + cash

--------------------------------------------------

**Prompt:**
How do I calculate annual return?

**AI Output:**
- percentage gain

--------------------------------------------------

**Prompt:**
How do I insert performance into BST?

**AI Output:**
- ticker + return

--------------------------------------------------

**Prompt:**
How do I display ranked strategy results?

**AI Output:**
- inorder traversal

--------------------------------------------------

**Prompt:**
How do I find best strategy parameter?

**AI Output:**
- BST findMax()

--------------------------------------------------

**Prompt:**
How do I store recent prices?

**AI Output:**
- vector<double>

--------------------------------------------------

**Prompt:**
How do I limit vector size?

**AI Output:**
- erase(begin())

--------------------------------------------------

**Prompt:**
How do I detect rallies?

**AI Output:**
- compare recent lows

--------------------------------------------------

**Prompt:**
How do I stop overspending?

**AI Output:**
- remaining budget checks

--------------------------------------------------

**Prompt:**
How do I keep code modular?

**AI Output:**
- separate cpp files

--------------------------------------------------

**Prompt:**
How do I organize headers?

**AI Output:**
- include folder

--------------------------------------------------

**Prompt:**
How do I avoid duplicate code?

**AI Output:**
- helper functions

--------------------------------------------------



# AI Prompts Log — ESE 224 Final Project

**Student Name:** Joseph Cabrera
**Student ID:**   114650793

\---

## Instructions

For every AI prompt you use during this project, add an entry below in the format shown.
This file gets submitted with your project and is the source of truth for your AI Documentation grade (10 pts).

You must also fill out the AI Usage Feedback Form after completing the project:
https://docs.google.com/forms/d/1NpkLKFrnvGgSsycC4Ba0IhCPSL\_MIHtGaU-X84D5CnY

\---

## Format

```
### Section: \[e.g., PriceHistory / CSVParser / GoldenCrossStrategy / Report Section 3]
\*\*Prompt:\*\*
\[paste your exact prompt here]

\*\*What you changed / why (if any):\*\*
\[what was wrong or insufficient, and what you modified]
```

\---

## Prompts

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

### Section: GoldenCrossStrategy.cpp

**Prompt:** 

\---

<!-- Add more entries above this line -->


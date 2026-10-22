Data files
==========

Daily price history from Yahoo Finance, one CSV file per symbol. The program
reads all three when you choose "[1] Load stock data from CSV" in the main menu.

  SPX.csv                    S&P 500 index   1927-12-30 to 2020-11-04
  AMZN.csv                   Amazon          1997-05-15 to 2022-03-24
  NVidia_stock_history.csv   NVIDIA          1999-01-22 to 2026-04-13

SPX.csv and AMZN.csv use Yahoo Finance's export column order:

  Date,Open,High,Low,Close,Adj Close,Volume

NVidia_stock_history.csv has the same columns in a different order:

  Date,Adj Close,Close,High,Low,Open,Volume

CSVParser finds each column by its header name, so the order doesn't matter.
It needs Date, Open, High, Low, Close and Volume, uses Close as the closing
price (Adj Close is ignored), skips rows it can't parse, and keeps only dates
from 2000-01-01 to 2020-01-01, so all three symbols cover the same 20 years.

#include "CSVParser.h"
#include <fstream> //read and write
#include <sstream> //split
#include <iostream>
using namespace std;

//helper for breaking up by comma
static int splitCSVLine(const string& line, string tokens[], int maxTokens) {
    string token;
    int count = 0;
    stringstream ss(line); //read from string

    while(count < maxTokens && getline(ss, token, ',')) {
        tokens[count] = token; //stops at each comma and stores piece
        count++;
    }

    return count;
}

PriceHistory* CSVParser::loadHistory(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return nullptr;
    }

    PriceHistory* history = new PriceHistory();

    string headerLine;
    if(!getline(file, headerLine)) {
        return history; //empty file after opening
    }

    const int MAX_COLUMNS = 64;
    string header[MAX_COLUMNS];
    int headerCount = splitCSVLine(headerLine, header, MAX_COLUMNS);

    int dateIndex = -1;
    int openIndex = -1;
    int highIndex = -1;
    int lowIndex = -1;
    int closeIndex = -1;
    int volumeIndex = -1;

    //dynamic mapping
    for(int i = 0; i < headerCount; i++){
          if (header[i] == "Date") {
            dateIndex = i;
        } else if (header[i] == "Open") {
            openIndex = i;
        } else if (header[i] == "High") {
            highIndex = i;
        } else if (header[i] == "Low") {
            lowIndex = i;
        } else if (header[i] == "Close") {
            closeIndex = i;
        } else if (header[i] == "Volume") {
            volumeIndex = i;
        }
    }

    //if missing
    if(dateIndex == -1 || openIndex == -1 || highIndex == -1 ||
        lowIndex == -1 || closeIndex == -1 || volumeIndex == -1) 
        {
        return history;
        }

    string line;
    const string STARTDATE = "2000-01-01";
    const string ENDDATE = "2020-01-01";
    
    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        string row[MAX_COLUMNS];
        int rowCount = splitCSVLine(line, row, MAX_COLUMNS);

        int maxIndex = dateIndex;
        if (openIndex > maxIndex) maxIndex = openIndex;
        if (highIndex > maxIndex) maxIndex = highIndex;
        if (lowIndex > maxIndex) maxIndex = lowIndex;
        if (closeIndex > maxIndex) maxIndex = closeIndex;
        if (volumeIndex > maxIndex) maxIndex = volumeIndex;

        if (rowCount <= maxIndex) {
            continue; // malformed row
        }

        try {
            string date = row[dateIndex];

            if (!dateInRange(date, STARTDATE, ENDDATE)) {
                continue;
            }
            //stod converts string into double
            double open = stod(row[openIndex]);
            double high = stod(row[highIndex]);
            double low = stod(row[lowIndex]);
            double close = stod(row[closeIndex]);
            long long volume = stoll(row[volumeIndex]);
            //add to linked list
            history->append(date, open, high, low, close, volume);
        }
        catch (...) {
            // Skip malformed numeric rows
            continue;
        }
    }

    file.close();
    return history;
}

//stoi converts strings to integers
bool CSVParser::dateInRange(const string& date, const string& start, const string& end) {
    return (date >= start && date <= end);
}

int CSVParser::extractYear(const string& date) {
    if (date.length() < 4) {
        return -1;
    }
    return stoi(date.substr(0, 4));
}

int CSVParser::extractMonth(const string& date) {
    if (date.length() < 7) {
        return -1;
    }
    return stoi(date.substr(5, 2));
}

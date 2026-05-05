#include "CSVParser.h"
#include <fstream> //read and write
#include <sstream> //split
#include <vector>
#include <iostream>
using namespace std;

//helper for breaking up by comma
static vector<string> splitCSVLine(const string& line) {
    vector <string> tokens;
    string token;
    stringstream ss(line); //read from string

    while(getline(ss, token, ',')) {
        tokens.push_back(token); //stops at each comma and stores piece into vector token
    }
    return tokens;
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
    vector<string> header = splitCSVLine(headerLine);

    int dateIndex = -1;
    int openIndex = -1;
    int highIndex = -1;
    int lowIndex = -1;
    int closeIndex = -1;
    int volumeIndex = -1;

    //dynamic mapping
    for(int i = 0; i < (int)header.size(); i++){
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
    const string STARTDATE = "1900-01-01";
    const string ENDDATE = "2025-01-01";
    
    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        vector<string> row = splitCSVLine(line);

        int maxIndex = dateIndex;
        if (openIndex > maxIndex) maxIndex = openIndex;
        if (highIndex > maxIndex) maxIndex = highIndex;
        if (lowIndex > maxIndex) maxIndex = lowIndex;
        if (closeIndex > maxIndex) maxIndex = closeIndex;
        if (volumeIndex > maxIndex) maxIndex = volumeIndex;

        if ((int)row.size() <= maxIndex) {
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
            long volume = stol(row[volumeIndex]);
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
#include "PriceHistory.h"
#include "PriceNode.h"
#include <iostream>
using namespace std;

//constructor
PriceHistory::PriceHistory() : head(nullptr), tail(nullptr), size(0){}

//destructor 
PriceHistory::~PriceHistory() {
    PriceNode*current = head;
    while(current != nullptr) {
        PriceNode* newNode = current->next; 
        delete current;
        current = newNode;
    }
    head = nullptr;
    tail = nullptr;
    size = 0; //size for list
}

//append(add new node to end of list)
void PriceHistory::append(const string& date, double open, double high, double low, double close, long volume) {
    
    PriceNode* newNode = new PriceNode(date, open, high, low, close, volume);
    if (head == nullptr) {
        head = newNode;
        tail = newNode;
    }
    else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    size++; 
}

//PriceNode* returns the actual node
PriceNode* PriceHistory::findByDate(const string& date) const {
    PriceNode*current = head;
    
    while (current != nullptr) {
        if(current->date == date) { //find by same date
            return current;
        }
        current = current->next;
    }
    return nullptr; //no results
}

//print function
void PriceHistory::printRange(const string& startDate, const string& endDate) const {
    PriceNode* current = head;

    while(current!= nullptr) {
        if(current->date >=startDate && current->date <= endDate) {
            cout << "Date: " << current->date
            << " | Open: " << current->open
                 << " | High: " << current->high
                 << " | Low: " << current->low
                 << " | Close: " << current->close
                 << " | Volume: " << current->volume
                 << endl;
        }
        current = current->next;
    }
}

//gethead
PriceNode* PriceHistory::getHead() const{
    return head;
}


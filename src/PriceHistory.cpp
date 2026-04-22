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

PriceNode* PriceHistory::getTail() const {
    return tail;
}

int PriceHistory::getSize() const {
    return size;
}


//move forward iterator
PriceHistory::Iterator::Iterator(PriceNode* node) : current(node){}
PriceHistory:: Iterator& PriceHistory::Iterator::operator++(){
    if (current != nullptr) {
        current = current->next;
    }    
    return *this;
}

PriceNode& PriceHistory::Iterator::operator*() {
    return *current;
}

bool PriceHistory::Iterator::operator!=(const Iterator& other) const {
    return current != other.current;
}

PriceHistory::Iterator PriceHistory::begin() const {
    return Iterator(head);
}

PriceHistory::Iterator PriceHistory::end() const {
    return Iterator(nullptr);
}


//reverse iterator
PriceHistory::ReverseIterator::ReverseIterator(PriceNode* node) : current(node) {}

PriceHistory::ReverseIterator& PriceHistory::ReverseIterator::operator++() {
    if (current != nullptr) {
        current = current->prev;
    }
    return *this;
}

PriceNode& PriceHistory::ReverseIterator::operator*() {
    return *current;
}

bool PriceHistory::ReverseIterator::operator!=(const ReverseIterator& other) const {
    return current != other.current;
}

PriceHistory::ReverseIterator PriceHistory::rbegin() const {
    return ReverseIterator(tail);
}

PriceHistory::ReverseIterator PriceHistory::rend() const {
    return ReverseIterator(nullptr);
}
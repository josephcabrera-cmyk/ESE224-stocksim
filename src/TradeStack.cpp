#include <iostream>
#include "TradeStack.h"
using namespace std;

//constructor
TradeStack::TradeStack():top(nullptr), size(0){}

TradeStack::~TradeStack() {
    while(!isEmpty()) {
        pop();
    }
}

//push
void TradeStack::push(const TradeRecord& record) {
    StackNode* newNode = new StackNode;
    newNode->data = record;
    newNode->next = top;
    top = newNode; //LIFO
    size++;
}

//pop
TradeRecord TradeStack::pop() {
    if (isEmpty()) {
        cout << "Error:" << endl;
        return TradeRecord{"", "", 0.0, 0, "", 0.0};
    }

    StackNode* temp = top;
    TradeRecord record = temp->data;
    top = top->next;
    delete temp;
    size--;
    return record;
}

TradeRecord TradeStack::peek() const {
    if(isEmpty()) {
        cout<< "Error: "<< endl;
        return TradeRecord{"","",0.0,0,"",0.0};
    }
    return top->data;
}

bool TradeStack::isEmpty() const{
    return top == nullptr;
}

int TradeStack::getSize() const {
    return size;
}

void TradeStack:: printAll() const{
    StackNode* current = top;

    while(current != nullptr) {
        cout << "Date: " <<current->data.date
         << " | Action: " << current->data.action
             << " | Ticker: " << current->data.ticker
             << " | Shares: " << current->data.shares
             << " | Price: " << current->data.price
             << " | Total: " << current->data.totalCost
             << endl;

        current = current->next;
    }
}
#include <iostream>
#include "../include/TradeStack.h"
using namespace std;

//constructor
TradeStack::TradeStack():top(nullptr), size(0){}

TradeStack::~TradeStack() {
    while(!isEmpty()) {
        pop();
    }
}

void TradeStack::push(const TradeRecord& record) {
    StackNode* newNode = new StackNode;
    newNode->data = record;
    newNode->next = top;
    top = newNode; //LIFO
    size++;
}

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

}

int TradeStack::getSize() const {

}

void TradeStack:: printAll() const{
    
}
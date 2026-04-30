#include "OrderQueue.h" //FIFO
#include <iostream>
using namespace std;

//constructor
OrderQueue::OrderQueue() : front(nullptr), back(nullptr), size(0) {
}

//destructor
OrderQueue::~OrderQueue() {
    while(!isEmpty) {
        dequeue();
    }
}

void OrderQueue::enqueue(const Order& order) {
    QueueNode* newNode = new QueueNode;
    newNode->data = order;
    newNode->next = nullptr;

    if(isEmpty()) {
        front = newNode;
        back = newNode;
    }else {
        back->next = newNode;
        back = newNode;
    }
    size++;
}


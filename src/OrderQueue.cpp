#include "OrderQueue.h" //FIFO
#include <iostream>
using namespace std;

//constructor
OrderQueue::OrderQueue() : front(nullptr), back(nullptr), size(0) {
}

//destructor
OrderQueue::~OrderQueue() {
    while (!isEmpty()) {
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

Order OrderQueue::dequeue() {
    if (isEmpty()) {
        cout << "Error: dequeue from empty OrderQueue." << endl;
        return Order{"", "", "", 0.0, 0, ""};
    }

    QueueNode* temp = front;
    Order order = temp->data;

    front = front->next;
    delete temp;
    size--;

    if (front == nullptr) {
        back = nullptr;
    }

    return order;
}

Order OrderQueue::peek() const {
    if (isEmpty()) {
        cout << "Error: peek from empty OrderQueue." << endl;
        return Order{"", "", "", 0.0, 0, ""};
    }

    return front->data;
}

bool OrderQueue::isEmpty() const {
    return front == nullptr;
}

int OrderQueue::getSize() const {
    return size;
}

void OrderQueue::printAll() const {
    QueueNode* current = front;

    while (current != nullptr) {
        cout << "Date: " << current->data.submittedDate
             << " | Type: " << current->data.type
             << " | Side: " << current->data.side
             << " | Ticker: " << current->data.ticker
             << " | Shares: " << current->data.shares
             << " | Target Price: " << current->data.targetPrice
             << endl;

        current = current->next;
    }
}
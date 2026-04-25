#include "CircularQueue.h"
#include <iostream>
using namespace std;

//constructor
CircularQueue::CircularQueue(int capacity):capacity(capacity), head(0), tail(0), count(0){
    buffer = new double[capacity];
}

//destructor
CircularQueue::~CircularQueue() {
    delete[] buffer;
}

//enqueue
void CircularQueue::enqueue(double value) {
    buffer[tail] = value;

    if (isFull()) {
        head = (head + 1) % capacity;
    }
    else {
        count++;
    }
    tail = (tail + 1) % capacity;
}

//Dequeue
double CircularQueue::dequeue() {
    if (isEmpty()) {
        cout << "Error";
        return 0.0;
    }
    double value = buffer[head];
    head = (head + 1) % capacity;
    count--;
    return value;
}

//peek
double CircularQueue::peek() const{
    if(isEmpty()) {
        cout << "Error";
        return 0.0;
    }
    return buffer[head];
}

//average
double CircularQueue::getAverage() const {
    if (isEmpty()) {
        return 0.0;
    }
    double sum = 0.0;
    for (int i = 0; i < count; i++) {
        int index = (head + i) % capacity;
        sum += buffer[index];
    }
    return sum / count;
}

//helpers
bool CircularQueue::isFull() const{
    return count == capacity;
}

bool CircularQueue::isEmpty() const{
    return count == 0;
}

int CircularQueue::getCount() const {
    return count;
}

int CircularQueue::getCapacity() const {
    return capacity;
}
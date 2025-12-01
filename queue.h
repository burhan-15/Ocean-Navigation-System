#ifndef QUEUE_H
#define QUEUE_H

#include <stdexcept>

template <typename T>
class Queue {
private:
    struct Node {
        T data;
        Node* next;
        Node(T val) : data(val), next(nullptr) {}
    };
    
    Node* front;
    Node* rear;
    int size;

public:
    Queue() : front(nullptr), rear(nullptr), size(0) {}
    
    ~Queue() {
        clear();
    }
    
    void enqueue(T val) {
        Node* newNode = new Node(val);
        if (isEmpty()) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        size++;
    }
    
    T dequeue() {
        if (isEmpty()) throw std::runtime_error("Queue is empty");
        Node* temp = front;
        T data = front->data;
        front = front->next;
        if (!front) rear = nullptr;
        delete temp;
        size--;
        return data;
    }
    
    T peek() const {
        if (isEmpty()) throw std::runtime_error("Queue is empty");
        return front->data;
    }
    
    bool isEmpty() const {
        return front == nullptr;
    }
    
    int getSize() const {
        return size;
    }
    
    void clear() {
        while (!isEmpty()) {
            dequeue();
        }
    }
};

#endif
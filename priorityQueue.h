#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <stdexcept>

template <typename T>
class PriorityQueue {
private:
    struct HeapNode {
        T data;
        int priority;
        HeapNode(T d, int p) : data(d), priority(p) {}
    };
    
    HeapNode** heap;
    int capacity;
    int size;
    
    void heapifyUp(int index);
    void heapifyDown(int index);
    void resize();
    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return 2 * i + 1; }
    int rightChild(int i) { return 2 * i + 2; }

public:
    PriorityQueue(int cap = 100);
    ~PriorityQueue();
    
    void push(T data, int priority);
    T pop();
    T top() const;
    bool isEmpty() const { return size == 0; }
    int getSize() const { return size; }
    void clear();
};

#endif
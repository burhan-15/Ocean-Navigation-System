#include "priorityQueue.h"

template <typename T>
PriorityQueue<T>::PriorityQueue(int cap) : capacity(cap), size(0) {
    heap = new HeapNode*[capacity];
}

template <typename T>
PriorityQueue<T>::~PriorityQueue() {
    clear();
    delete[] heap;
}

template <typename T>
void PriorityQueue<T>::resize() {
    capacity *= 2;
    HeapNode** newHeap = new HeapNode*[capacity];
    for (int i = 0; i < size; i++) {
        newHeap[i] = heap[i];
    }
    delete[] heap;
    heap = newHeap;
}

template <typename T>
void PriorityQueue<T>::heapifyUp(int index) {
    while (index > 0 && heap[index]->priority < heap[parent(index)]->priority) {
        HeapNode* temp = heap[index];
        heap[index] = heap[parent(index)];
        heap[parent(index)] = temp;
        index = parent(index);
    }
}

template <typename T>
void PriorityQueue<T>::heapifyDown(int index) {
    int smallest = index;
    int left = leftChild(index);
    int right = rightChild(index);
    
    if (left < size && heap[left]->priority < heap[smallest]->priority)
        smallest = left;
    if (right < size && heap[right]->priority < heap[smallest]->priority)
        smallest = right;
    
    if (smallest != index) {
        HeapNode* temp = heap[index];
        heap[index] = heap[smallest];
        heap[smallest] = temp;
        heapifyDown(smallest);
    }
}

template <typename T>
void PriorityQueue<T>::push(T data, int priority) {
    if (size >= capacity) resize();
    heap[size] = new HeapNode(data, priority);
    heapifyUp(size);
    size++;
}

template <typename T>
T PriorityQueue<T>::pop() {
    if (isEmpty()) throw std::runtime_error("Priority queue is empty");
    T result = heap[0]->data;
    delete heap[0];
    heap[0] = heap[size - 1];
    size--;
    if (size > 0) heapifyDown(0);
    return result;
}

template <typename T>
T PriorityQueue<T>::top() const {
    if (isEmpty()) throw std::runtime_error("Priority queue is empty");
    return heap[0]->data;
}

template <typename T>
void PriorityQueue<T>::clear() {
    for (int i = 0; i < size; i++) {
        delete heap[i];
    }
    size = 0;
}
#include <iostream>
#include <stdexcept>
#include "linkedList.h"

template <typename T>
LinkedList<T>::Node::Node(T val) : data(val), next(nullptr) {}

template <typename T>
LinkedList<T>::LinkedList() : head(nullptr), tail(nullptr), size(0) {}

template <typename T>
LinkedList<T>::~LinkedList() {
    clear();
}

template <typename T>
void LinkedList<T>::insertEnd(T val) {
    Node* newNode = new Node(val);
    if (!head) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    size++;
}

template <typename T>
void LinkedList<T>::insertFront(T val) {
    Node* newNode = new Node(val);
    if (!head) {
        head = tail = newNode;
    } else {
        newNode->next = head;
        head = newNode;
    }
    size++;
}

template <typename T>
bool LinkedList<T>::isEmpty() const {
    return head == nullptr;
}

template <typename T>
int LinkedList<T>::getSize() const {
    return size;
}

template <typename T>
void LinkedList<T>::clear() {
    Node* current = head;
    while (current != nullptr) {
        Node* tmp = current;
        current = current->next;
        delete tmp;
    }
    head = tail = nullptr;
    size = 0;
}

template <typename T>
T LinkedList<T>::get(int index) {
    if (index < 0 || index >= size) throw std::out_of_range("Index out of range");
    Node* current = head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    return current->data;
}



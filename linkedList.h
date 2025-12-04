#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdexcept>

template <typename T>
class LinkedList {
public:
    struct Node {
        T data;
        Node* next;
        Node(T val) : data(val), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int size;

    LinkedList() : head(nullptr), tail(nullptr), size(0) {}
    ~LinkedList() { clear(); }

    void insertEnd(T val) {
        Node* newNode = new Node(val);
        if (!head) head = tail = newNode;
        else { tail->next = newNode; tail = newNode; }
        size++;
    }

    void insertFront(T val) {
        Node* newNode = new Node(val);
        if (!head) head = tail = newNode;
        else { newNode->next = head; head = newNode; }
        size++;
    }

    bool isEmpty() const { return head == nullptr; }

    int getSize() const { return size; }

    void clear() {
        Node* current = head;
        while (current) {
            Node* tmp = current;
            current = current->next;
            delete tmp;
        }
        head = tail = nullptr;
        size = 0;
    }

    T get(int index) const {
        if (index < 0 || index >= size) throw std::out_of_range("Index out of range");
        Node* current = head;
        for (int i = 0; i < index; i++) current = current->next;
        return current->data;
    }
};

#endif

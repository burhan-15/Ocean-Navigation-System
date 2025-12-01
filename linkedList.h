#ifndef LINKEDLIST_H
#define LINKEDLIST_H

template <typename T>
class LinkedList {
public:
    struct Node {
        T data;
        Node* next;
        Node(T val);
    };

    Node* head;
    Node* tail;
    int size;

    LinkedList();
    ~LinkedList();

    void insertEnd(T val);
    void insertFront(T val);
    bool isEmpty() const;
    int getSize() const;
    void clear();
    T get(int index);
};

#endif

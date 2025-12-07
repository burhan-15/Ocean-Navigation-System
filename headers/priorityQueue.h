#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <stdexcept>

template <typename T>
class PriorityQueue {
private:
    struct Node {
        T data;
        int priority;
        Node* next;

        Node(T d, int p) : data(d), priority(p), next(nullptr) {}
    };

    Node* head;    // always points to smallest priority
    int size;

public:
    PriorityQueue() : head(nullptr), size(0) {}

    ~PriorityQueue() {
        clear();
    }

    void push(T data, int priority) {
        Node* newNode = new Node(data, priority);

        // Insert at beginning if queue empty or smaller priority
        if (!head || priority < head->priority) {
            newNode->next = head;
            head = newNode;
        } else {
            // Otherwise find correct sorted position
            Node* current = head;
            while (current->next && current->next->priority <= priority) {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
        }

        size++;
    }

    T pop() {
        if (!head) throw std::runtime_error("Priority queue is empty");

        Node* temp = head;
        T result = temp->data;
        head = head->next;
        delete temp;

        size--;
        return result;
    }

    T top() const {
        if (!head) throw std::runtime_error("Priority queue is empty");
        return head->data;
    }

    bool isEmpty() const {
        return head == nullptr;
    }

    int getSize() const {
        return size;
    }

    void clear() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        size = 0;
    }
};

#endif

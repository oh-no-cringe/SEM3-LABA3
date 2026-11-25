#ifndef QUEUE_H
#define QUEUE_H

#include <string>

template <class T> class Queue {
private:
    friend class Storage;
    struct Node {
        T value;
        Node* next;

        Node(const T& new_value, Node* next_node) : value(new_value), next(next_node) {
        }
    };

    Node* head = nullptr;
    Node* tail = nullptr;

public:
    Queue() = default;
    ~Queue();
    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;

    void push(const T& value);
    T pop();
    bool empty() const;
    void print() const;
};

#include "queue.tpp"

#endif

#ifndef LR3_QUEUE_TPP
#define LR3_QUEUE_TPP

#include "queue.h"
#include <iostream>
#include <stdexcept>

template <class T> Queue<T>::~Queue() {
    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

template <class T> void Queue<T>::push(const T& value) {
    Node* new_node = new Node{value, nullptr};
    if (tail == nullptr) {
        head = new_node;
    } else {
        tail->next = new_node;
    }
    tail = new_node;
}

template <class T> T Queue<T>::pop() {
    if (empty()) {
        throw std::out_of_range("Очередь пуста");
    }
    T value = head->value;
    Node* temp = head;
    if (head == tail) {
        head = nullptr;
        tail = nullptr;
    } else {
        head = head->next;
    }
    delete temp;
    return value;
}

template <class T> bool Queue<T>::empty() const {
    return head == nullptr;
}

template <class T> void Queue<T>::print() const {
    std::cout << '[';
    bool first = true;
    Node* node = head;
    while (node != nullptr) {
        if (!first) {
            std::cout << ", ";
        }
        std::cout << node->value;
        first = false;
        node = node->next;
    }
    std::cout << "]\n";
}

#endif

#ifndef STACK_H
#define STACK_H
#include <iostream>
#include <string>

class Stack {
private:
    struct Node {
        std::string value;
        Node* next;
        Node(const std::string& value, Node* n) : value(value), next(n) {
        }
    };
    Node* head;
    void clear();

public:
    Stack() {
        head = nullptr;
    }
    ~Stack() {
        clear();
    }

    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;

    void push(const std::string& value);

    std::string pop();
    bool empty() const;

    void print() const;
};

#endif

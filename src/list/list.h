#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <string>

class List {
private:
    struct Node {
        std::string value;
        Node *next, *prev;
        Node(const std::string& value, Node* n, Node* p) : value(value), next(n), prev(p) {
        }
    };
    Node* head;
    Node* tail;

    void clear();

public:
    List() {
        head = nullptr;
        tail = nullptr;
    }
    ~List() {
        clear();
    }

    List(const List&) = delete;
    List& operator=(const List&) = delete;

    void push_front(const std::string& value);
    void push_back(const std::string& value);
    void push_index(const std::string& value, int index);

    void pop_front();
    void pop_back();
    void pop_index(int index);
    bool erase_value(const std::string& value);

    bool insert_before(const std::string& target, const std::string& value);
    bool insert_after(const std::string& target, const std::string& value);
    bool erase_before(const std::string& target);
    bool erase_after(const std::string& target);

    bool find(const std::string& value) const;
    void print() const;
    bool empty() const;
};

#endif

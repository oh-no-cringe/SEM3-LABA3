#include "stack.h"
#include <iostream>
#include <stdexcept>

using namespace std;

void Stack::push(const string& value) {
    Node* new_node = new Node{value, head};
    head = new_node;
}

string Stack::pop() {
    if (!head) {
        throw out_of_range("stack is empty");
    }
    string value = head->value;
    Node* temp = head;
    head = temp->next;
    delete temp;
    return value;
}

bool Stack::empty() const {
    return head == nullptr;
}

void Stack::print() const {
    cout << "[";
    bool first = true;
    Node* current = head;
    while (current) {
        if (!first) {
            cout << ", ";
        }
        cout << current->value;
        first = false;
        current = current->next;
    }
    cout << "]\n";
}

void Stack::clear() {
    while (head) {
        Node* temp = head;
        head = temp->next;
        delete temp;
    }
}

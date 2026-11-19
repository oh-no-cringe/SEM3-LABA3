#include "forward_list.h"
#include <iostream>

using namespace std;

void ForwardList::push_front(const string& value) {
    head = new Node{value, head};
}

void ForwardList::push_back(const string& value) {
    Node* new_node = new Node{value, nullptr};
    if (!head) {
        head = new_node;
        return;
    }
    Node* current = head;
    while (current->next) {
        current = current->next;
    }
    current->next = new_node;
}

void ForwardList::push_index(const string& value, int index) {
    if (index < 0) {
        return;
    }
    if (index == 0) {
        push_front(value);
        return;
    }
    Node* current = head;
    for (int i = 0; i < index - 1 && current; ++i) {
        current = current->next;
    }
    if (!current) {
        return;
    }
    current->next = new Node{value, current->next};
}

void ForwardList::pop_front() {
    if (!head) {
        return;
    }
    Node* temp = head;
    head = temp->next;
    delete temp;
}

void ForwardList::pop_back() {
    if (!head) {
        return;
    }

    if (!head->next) {
        Node* temp = head;
        delete temp;
        head = nullptr;
        return;
    }

    Node* current = head;
    while (current->next->next) {
        current = current->next;
    }
    Node* temp = current->next;
    delete temp;
    current->next = nullptr;
}

void ForwardList::pop_index(int index) {
    if (index < 0) {
        return;
    }
    if (!head) {
        return;
    }

    if (index == 0) {
        pop_front();
        return;
    }

    Node* current = head;
    for (int i = 0; i < index - 1 && current->next; ++i) {
        current = current->next;
    }
    if (!current->next) {
        return;
    }

    Node* temp = current->next;
    current->next = temp->next;
    delete temp;
}

bool ForwardList::find(const string& value) const {
    Node* current = head;
    while (current) {
        if (current->value == value) {
            return true;
        }
        current = current->next;
    }
    return false;
}

bool ForwardList::empty() const {
    return head == nullptr;
}

void ForwardList::print() const {
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

void ForwardList::clear() {
    while (head) {
        Node* temp = head;
        head = temp->next;
        delete temp;
    }
}

bool ForwardList::insert_before(const string& target, const string& value) {
    Node* previous = nullptr;
    Node* current = head;
    while (current && current->value != target) {
        previous = current;
        current = current->next;
    }
    if (!current) {
        return false;
    }
    Node* node = new Node{value, current};
    if (previous) {
        previous->next = node;
    } else {
        head = node;
    }
    return true;
}

bool ForwardList::insert_after(const string& target, const string& value) {
    Node* current = head;
    while (current && current->value != target) {
        current = current->next;
    }
    if (!current) {
        return false;
    }
    current->next = new Node{value, current->next};
    return true;
}

bool ForwardList::erase_before(const string& target) {
    Node* before_previous = nullptr;
    Node* previous = nullptr;
    Node* current = head;
    while (current && current->value != target) {
        before_previous = previous;
        previous = current;
        current = current->next;
    }
    if (!current || !previous) {
        return false;
    }
    if (before_previous) {
        before_previous->next = current;
    } else {
        head = current;
    }
    Node* temp = previous;
    delete temp;
    return true;
}

bool ForwardList::erase_after(const string& target) {
    Node* current = head;
    while (current && current->value != target) {
        current = current->next;
    }
    if (!current || !current->next) {
        return false;
    }
    Node* temp = current->next;
    current->next = temp->next;
    delete temp;
    return true;
}

bool ForwardList::erase_value(const string& value) {
    Node* previous = nullptr;
    Node* current = head;
    while (current && current->value != value) {
        previous = current;
        current = current->next;
    }
    if (!current) {
        return false;
    }
    if (previous) {
        previous->next = current->next;
    } else {
        head = current->next;
    }
    Node* temp = current;
    delete temp;
    return true;
}

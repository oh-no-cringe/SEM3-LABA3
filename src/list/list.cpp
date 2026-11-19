#include "list.h"
#include <iostream>

using namespace std;

void List::push_front(const string& value) {
    Node* new_node = new Node{value, head, nullptr};
    if (head != nullptr) {
        head->prev = new_node;

    } else {
        tail = new_node;
    }
    head = new_node;
}

void List::push_back(const string& value) {
    Node* new_node = new Node{value, nullptr, tail};
    if (tail != nullptr) {
        tail->next = new_node;
    } else {
        head = new_node;
    }
    tail = new_node;
}

void List::push_index(const string& value, int index) {
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
        push_back(value);
        return;
    }

    Node* new_node = new Node{value, current->next, current};

    if (current->next) {
        current->next->prev = new_node;
    } else {
        tail = new_node;
    }

    current->next = new_node;
}

void List::pop_front() {
    if (!head) {
        return;
    }

    Node* temp = head;
    head = temp->next;

    if (head) {
        head->prev = nullptr;
    } else {
        tail = nullptr;
    }

    delete temp;
}

void List::pop_back() {
    if (!tail) {
        return;
    }

    Node* temp = tail;
    tail = temp->prev;

    if (tail) {
        tail->next = nullptr;
    } else {
        head = nullptr;
    }

    delete temp;
}

void List::pop_index(int index) {
    if (!head) {
        return;
    }
    if (index < 0) {
        return;
    }

    if (index == 0) {
        pop_front();
        return;
    }

    Node* current = head;
    for (int i = 0; i < index && current; ++i) {
        current = current->next;
    }
    if (!current) {
        return;
    }

    if (current == tail) {
        pop_back();
        return;
    }

    if (current->prev) {
        current->prev->next = current->next;
    }

    if (current->next) {
        current->next->prev = current->prev;
    }

    Node* temp = current;
    delete temp;
}

bool List::find(const string& value) const {
    Node* current = head;
    while (current) {
        if (current->value == value) {
            return true;
        }
        current = current->next;
    }
    return false;
}

bool List::empty() const {
    return head == nullptr;
}

void List::print() const {
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

void List::clear() {
    while (head) {
        Node* temp = head;
        head = temp->next;
        delete temp;
    }
    tail = nullptr;
}

bool List::insert_before(const string& target, const string& value) {
    Node* current = head;
    while (current && current->value != target) {
        current = current->next;
    }
    if (!current) {
        return false;
    }
    Node* node = new Node{value, current, current->prev};
    if (current->prev) {
        current->prev->next = node;
    } else {
        head = node;
    }
    current->prev = node;
    return true;
}

bool List::insert_after(const string& target, const string& value) {
    Node* current = head;
    while (current && current->value != target) {
        current = current->next;
    }
    if (!current) {
        return false;
    }
    Node* node = new Node{value, current->next, current};
    if (current->next) {
        current->next->prev = node;
    } else {
        tail = node;
    }
    current->next = node;
    return true;
}

bool List::erase_before(const string& target) {
    Node* current = head;
    while (current && current->value != target) {
        current = current->next;
    }
    if (!current || !current->prev) {
        return false;
    }
    Node* temp = current->prev;
    current->prev = temp->prev;
    if (temp->prev) {
        temp->prev->next = current;
    } else {
        head = current;
    }
    delete temp;
    return true;
}

bool List::erase_after(const string& target) {
    Node* current = head;
    while (current && current->value != target) {
        current = current->next;
    }
    if (!current || !current->next) {
        return false;
    }
    Node* temp = current->next;
    current->next = temp->next;
    if (temp->next) {
        temp->next->prev = current;
    } else {
        tail = current;
    }
    delete temp;
    return true;
}

bool List::erase_value(const string& value) {
    Node* current = head;
    while (current && current->value != value) {
        current = current->next;
    }
    if (!current) {
        return false;
    }
    if (current->prev) {
        current->prev->next = current->next;
    } else {
        head = current->next;
    }
    if (current->next) {
        current->next->prev = current->prev;
    } else {
        tail = current->prev;
    }
    Node* temp = current;
    delete temp;
    return true;
}

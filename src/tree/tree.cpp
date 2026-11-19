#include "tree.h"
#include "queue/queue.h"
#include <iostream>

using namespace std;

Tree::~Tree() {
    clear(root);
}

void Tree::insert(int value) {
    if (root == nullptr) {
        root = new Node{value, nullptr, nullptr};
        return;
    }
    Queue<Node*> next;
    next.push(root);
    while (!next.empty()) {
        Node* node = next.pop();
        if (node->left == nullptr) {
            node->left = new Node{value, nullptr, nullptr};
            return;
        }
        if (node->right == nullptr) {
            node->right = new Node{value, nullptr, nullptr};
            return;
        }
        next.push(node->left);
        next.push(node->right);
    }
}

bool Tree::find_value(const Node* node, int value) const {
    if (node == nullptr) {
        return false;
    }
    if (node->value == value) {
        return true;
    }
    return find_value(node->left, value) || find_value(node->right, value);
}

bool Tree::search(int value) const {
    return find_value(root, value);
}

bool Tree::is_complete() const {
    Queue<const Node*> next;
    next.push(root);
    bool gap = false;
    while (!next.empty()) {
        const Node* node = next.pop();
        if (node == nullptr) {
            gap = true;
        } else {
            if (gap) {
                return false;
            }
            next.push(node->left);
            next.push(node->right);
        }
    }
    return true;
}

bool Tree::empty() const {
    return root == nullptr;
}

static void print_value(int value, bool& first) {
    if (!first) {
        cout << ", ";
    }
    cout << value;
    first = false;
}

void Tree::print() const {
    cout << '[';
    Queue<const Node*> next;
    if (root != nullptr) {
        next.push(root);
    }
    bool first = true;
    while (!next.empty()) {
        const Node* node = next.pop();
        print_value(node->value, first);
        if (node->left != nullptr) {
            next.push(node->left);
        }
        if (node->right != nullptr) {
            next.push(node->right);
        }
    }
    cout << "]\n";
}

void Tree::clear(Node* temp) {
    if (temp == nullptr) {
        return;
    }
    clear(temp->left);
    clear(temp->right);
    delete temp;
}

void Tree::print_pre(const Node* node, bool& first) const {
    if (node == nullptr) {
        return;
    }
    print_value(node->value, first);
    print_pre(node->left, first);
    print_pre(node->right, first);
}

void Tree::print_in(const Node* node, bool& first) const {
    if (node == nullptr) {
        return;
    }
    print_in(node->left, first);
    print_value(node->value, first);
    print_in(node->right, first);
}

void Tree::print_post(const Node* node, bool& first) const {
    if (node == nullptr) {
        return;
    }
    print_post(node->left, first);
    print_post(node->right, first);
    print_value(node->value, first);
}

void Tree::print_preorder() const {
    cout << '[';
    bool first = true;
    print_pre(root, first);
    cout << "]\n";
}

void Tree::print_inorder() const {
    cout << '[';
    bool first = true;
    print_in(root, first);
    cout << "]\n";
}

void Tree::print_postorder() const {
    cout << '[';
    bool first = true;
    print_post(root, first);
    cout << "]\n";
}

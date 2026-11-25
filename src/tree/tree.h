#ifndef TREE_H
#define TREE_H

class Tree {
private:
    friend class Storage;
    struct Node {
        int value;
        Node* left;
        Node* right;

        Node(int number, Node* left_node, Node* right_node)
            : value(number), left(left_node), right(right_node) {
        }
    };

    Node* root = nullptr;

    void clear(Node* temp);
    bool find_value(const Node* node, int value) const;
    void print_pre(const Node* node, bool& first) const;
    void print_in(const Node* node, bool& first) const;
    void print_post(const Node* node, bool& first) const;

public:
    Tree() = default;
    ~Tree();
    Tree(const Tree&) = delete;
    Tree& operator=(const Tree&) = delete;

    void insert(int value);
    bool search(int value) const;
    bool is_complete() const;
    bool empty() const;

    void print() const;
    void print_preorder() const;
    void print_inorder() const;
    void print_postorder() const;
};

#endif

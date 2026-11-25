#ifndef FORWARD_LIST
#define FORWARD_LIST

#include <string>

class ForwardList {
private:
    friend class Storage;
    struct Node {
        std::string value;
        Node* next;
        Node(const std::string& value, Node* n) : value(value), next(n) {
        }
    };

    Node* head;

    void clear();

public:
    ForwardList() {
        head = nullptr;
    }
    ~ForwardList() {
        clear();
    }

    ForwardList(const ForwardList&) = delete;
    ForwardList& operator=(const ForwardList&) = delete;

    void push_front(const std::string& value);
    void push_back(const std::string& value);
    void push_index(const std::string& value, int index);

    void pop_front();
    void pop_back();
    void pop_index(int index);

    bool insert_before(const std::string& target, const std::string& value);
    bool insert_after(const std::string& target, const std::string& value);
    bool erase_value(const std::string& value);
    bool erase_before(const std::string& target);
    bool erase_after(const std::string& target);

    bool find(const std::string& value) const;
    void print() const;
    bool empty() const;
};

#endif

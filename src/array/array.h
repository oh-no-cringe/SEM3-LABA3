#ifndef ARRAY_H
#define ARRAY_H

#include <cstddef>
#include <string>

class Array {
private:
    friend class Storage;
    std::string* data;
    std::size_t size = 0;
    std::size_t cap = 4;

    void grow();

public:
    Array();
    ~Array();
    Array(const Array&) = delete;
    Array& operator=(const Array&) = delete;

    void push_back(const std::string& value);
    void push_index(const std::string& value, std::size_t index);
    std::string get(std::size_t index) const;
    void set(std::size_t index, const std::string& value);
    void pop_index(std::size_t index);
    void clear();
    void swap(Array& other);
    void print() const;

    std::size_t length() const {
        return size;
    }

    bool empty() const {
        return size == 0;
    }
};

#endif

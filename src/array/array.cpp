#include "array.h"
#include <iostream>
#include <stdexcept>
#include <utility>

using namespace std;

Array::Array() {
    data = new string[cap];
}

Array::~Array() {
    delete[] data;
}

void Array::grow() {
    size_t new_cap = cap * 2;
    if (new_cap <= cap) {
        throw length_error("Массив слишком большой");
    }
    string* new_data = new string[new_cap];
    for (size_t i = 0; i < size; ++i) {
        new_data[i].swap(data[i]);
    }
    delete[] data;
    data = new_data;
    cap = new_cap;
}

void Array::push_back(const string& value) {
    string copy = value;
    if (size == cap) {
        grow();
    }
    data[size] = copy;
    ++size;
}

void Array::push_index(const string& value, size_t index) {
    if (index > size) {
        throw out_of_range("Нет такого индекса");
    }
    string copy = value;
    if (size == cap) {
        grow();
    }
    for (size_t i = size; i > index; --i) {
        data[i] = data[i - 1];
    }
    data[index] = copy;
    ++size;
}

string Array::get(size_t index) const {
    if (index >= size) {
        throw out_of_range("Нет такого индекса");
    }
    return data[index];
}

void Array::set(size_t index, const string& value) {
    if (index >= size) {
        throw out_of_range("Нет такого индекса");
    }
    data[index] = value;
}

void Array::pop_index(size_t index) {
    if (index >= size) {
        return;
    }
    for (size_t i = index; i + 1 < size; ++i) {
        data[i] = data[i + 1];
    }
    --size;
    data[size].clear();
}

void Array::clear() {
    for (size_t i = 0; i < size; ++i) {
        data[i].clear();
    }
    size = 0;
}

void Array::swap(Array& other) {
    std::swap(data, other.data);
    std::swap(size, other.size);
    std::swap(cap, other.cap);
}

void Array::print() const {
    cout << '[';
    for (size_t i = 0; i < size; ++i) {
        if (i > 0) {
            cout << ", ";
        }
        cout << data[i];
    }
    cout << "]\n";
}

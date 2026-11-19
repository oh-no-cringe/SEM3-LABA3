#include "input.h"
#include <cctype>
#include <iomanip>
#include <iostream>
#include <stdexcept>

using namespace std;

string read_word() {
    string value;
    if (!(cin >> quoted(value))) {
        throw runtime_error("Нужно значение");
    }
    int next = cin.peek();
    if (next != char_traits<char>::eof() && !isspace(static_cast<unsigned char>(next))) {
        throw runtime_error("Между аргументами нужен пробел");
    }
    return value;
}

int read_number() {
    string text = read_word();
    size_t read = 0;
    int value = stoi(text, &read);
    if (read != text.size()) {
        throw runtime_error("Нужно целое число");
    }
    return value;
}

int read_index() {
    int index = read_number();
    if (index < 0) {
        throw runtime_error("Индекс должен быть неотрицательным");
    }
    return index;
}

void print_bool(bool value) {
    if (value) {
        cout << "TRUE\n";
    } else {
        cout << "FALSE\n";
    }
}

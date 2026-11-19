#include "hash_table.h"
#include <functional>
#include <iomanip>
#include <iostream>
#include <stdexcept>

using namespace std;

HashTable::HashTable(bool use_double) : double_hash(use_double) {
    for (int i = 0; i < 8; ++i) {
        keys.push_back("");
        data.push_back("");
    }
}

size_t HashTable::hash_pos(const string& key) const {
    return hash<string>{}(key) % keys.length();
}

size_t HashTable::hash_step(const string& key) const {
    if (!double_hash) {
        return 1;
    }
    size_t number = 0;
    for (unsigned char letter : key) {
        number = number * 131 + letter;
    }
    // Размер таблицы — степень двойки, поэтому шаг делаем нечётным.
    return 2 * (number % (keys.length() / 2)) + 1;
}

size_t HashTable::find_pos(const string& key) const {
    size_t size = keys.length();
    size_t pos = hash_pos(key);
    size_t step = hash_step(key);
    size_t deleted = size;

    for (size_t i = 0; i < size; ++i) {
        string cell = keys.get(pos);
        if (cell == "+" + key) {
            return pos;
        }
        if (cell.empty()) {
            if (deleted != size) {
                return deleted;
            }
            return pos;
        }
        if (cell == "-" && deleted == size) {
            deleted = pos;
        }
        pos = (pos + step) % size;
    }
    return deleted;
}

void HashTable::set(const string& key, const string& value) {
    size_t pos = find_pos(key);
    if (pos < keys.length() && keys.get(pos) == "+" + key) {
        data.set(pos, value);
        return;
    }
    size_t size = keys.length();
    if (count >= size / 2 || pos == size) {
        rebuild(true);
        pos = find_pos(key);
    } else if (used >= size - size / 4 && keys.get(pos).empty()) {
        rebuild(false);
        pos = find_pos(key);
    }

    if (keys.get(pos).empty()) {
        ++used;
    }
    data.set(pos, value);
    keys.set(pos, "+" + key);
    ++count;
}

bool HashTable::get(const string& key, string& value) const {
    size_t pos = find_pos(key);
    if (pos == keys.length() || keys.get(pos) != "+" + key) {
        return false;
    }
    value = data.get(pos);
    return true;
}

bool HashTable::has(const string& key) const {
    size_t pos = find_pos(key);
    return pos < keys.length() && keys.get(pos) == "+" + key;
}

void HashTable::del(const string& key) {
    size_t pos = find_pos(key);
    if (pos < keys.length() && keys.get(pos) == "+" + key) {
        keys.set(pos, "-");
        data.set(pos, "");
        --count;
    }
}

void HashTable::names(Array& result) const {
    result.clear();
    for (size_t i = 0; i < keys.length(); ++i) {
        string key = keys.get(i);
        if (!key.empty() && key.front() == '+') {
            result.push_back(key.substr(1));
        }
    }
}

void HashTable::rebuild(bool grow) {
    size_t new_size = keys.length();
    if (grow) {
        new_size *= 2;
        if (new_size <= keys.length()) {
            throw length_error("Хеш-таблица слишком большая");
        }
    }

    HashTable temp(double_hash);
    while (temp.keys.length() < new_size) {
        temp.keys.push_back("");
        temp.data.push_back("");
    }
    for (size_t i = 0; i < keys.length(); ++i) {
        string key = keys.get(i);
        if (!key.empty() && key.front() == '+') {
            temp.set(key.substr(1), data.get(i));
        }
    }

    keys.swap(temp.keys);
    data.swap(temp.data);
    count = temp.count;
    used = temp.used;
}

void HashTable::print() const {
    Array keys;
    names(keys);
    cout << '[';
    for (size_t i = 0; i < keys.length(); ++i) {
        if (i > 0) {
            cout << ", ";
        }
        string key = keys.get(i);
        string value;
        get(key, value);
        cout << quoted(key) << ": " << quoted(value);
    }
    cout << "]\n";
}

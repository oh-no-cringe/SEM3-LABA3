#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "array/array.h"

class HashTable {
private:
    friend class Storage;
    Array keys;
    Array data;
    std::size_t count = 0;
    std::size_t used = 0;
    bool double_hash;

    std::size_t hash_pos(const std::string& key) const;
    std::size_t hash_step(const std::string& key) const;
    std::size_t find_pos(const std::string& key) const;
    void rebuild(bool grow);

public:
    explicit HashTable(bool use_double = false);
    void set(const std::string& key, const std::string& value);
    bool get(const std::string& key, std::string& value) const;
    bool has(const std::string& key) const;
    void del(const std::string& key);
    void names(Array& result) const;
    void print() const;
};

#endif

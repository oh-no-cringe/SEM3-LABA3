#ifndef STORAGE_H
#define STORAGE_H
#include "array/array.h"
#include "forward_list/forward_list.h"
#include "list/list.h"
#include "stack/stack.h"
#include "queue/queue.h"
#include "tree/tree.h"
#include "hash/hash_table.h"
#include <string>
class Storage {
public:
    static void Values(const Array& object, Array& values);
    static void Restore(Array& object, const Array& values);
    static void Values(const ForwardList& object, Array& values);
    static void Restore(ForwardList& object, const Array& values);
    static void Values(const List& object, Array& values);
    static void Restore(List& object, const Array& values);
    static void Values(const Stack& object, Array& values);
    static void Restore(Stack& object, const Array& values);
    static void Values(const Queue<std::string>& object, Array& values);
    static void Restore(Queue<std::string>& object, const Array& values);
    static void Values(const HashTable& object, Array& values);
    static void Restore(HashTable& object, const Array& values);
    static void Values(const Tree& object, Array& values);
    static void Restore(Tree& object, const Array& values);
};
void WriteFile(const std::string& path, const std::string& kind, const Array& values, bool binary);
void ReadFile(const std::string& path, const std::string& kind, Array& values, bool binary);
template <class T>
void Save(const T& object, const std::string& path, const std::string& kind, bool binary) {
    Array values;
    Storage::Values(object, values);
    WriteFile(path, kind, values, binary);
}
template <class T>
void Load(T& object, const std::string& path, const std::string& kind, bool binary) {
    Array values;
    ReadFile(path, kind, values, binary);
    Storage::Restore(object, values);
}
#endif

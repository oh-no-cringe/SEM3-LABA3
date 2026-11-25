#include "storage.h"
#include <fstream>
#include <cstdint>
#include <stdexcept>
using namespace std;
static void Put(ostream& out, uint32_t n) {
    for (int i = 0; i < 4; ++i) {
        out.put(static_cast<char>((n >> (i * 8)) & 255));
    }
}
static uint32_t Get(istream& in) {
    uint32_t n = 0;
    for (int i = 0; i < 4; ++i) {
        int c = in.get();
        if (c == EOF) {
            throw runtime_error("Обрезанный файл");
        }
        n |= uint32_t(c) << (i * 8);
    }
    return n;
}
static uint32_t Number(istream& in) {
    string line;
    if (!getline(in, line) || line.empty() ||
        line.find_first_not_of("0123456789") != string::npos) {
        throw runtime_error("Неверное число в файле");
    }
    unsigned long long n = stoull(line);
    if (n > 1048576) {
        throw runtime_error("Слишком большой размер");
    }
    return uint32_t(n);
}
static void WriteString(ostream& out, const string& s, bool binary) {
    if (s.size() > 1048576) {
        throw runtime_error("Слишком длинная строка");
    }
    if (binary) {
        Put(out, uint32_t(s.size()));
    } else {
        out << s.size() << '\n';
    }
    out.write(s.data(), s.size());
    if (!binary) {
        out.put('\n');
    }
}
static string ReadString(istream& in, bool binary) {
    uint32_t n;
    if (binary) {
        n = Get(in);
    } else {
        n = Number(in);
    }
    if (n > 1048576) {
        throw runtime_error("Слишком длинная строка");
    }
    string s(n, '\0');
    if (!in.read(s.data(), n)) {
        throw runtime_error("Обрезанная строка");
    }
    if (!binary && in.get() != '\n') {
        throw runtime_error("Нет разделителя");
    }
    return s;
}
void WriteFile(const string& path, const string& kind, const Array& values, bool binary) {
    ofstream out(path, ios::binary);
    if (!out) {
        throw runtime_error("Не открыть файл для записи");
    }
    if (values.length() > 100000) {
        throw runtime_error("Слишком много элементов");
    }
    if (binary) {
        out << "LR3BIN1\n";
    } else {
        out << "LR3TEXT1\n";
    }
    WriteString(out, kind, binary);
    if (binary) {
        Put(out, uint32_t(values.length()));
    } else {
        out << values.length() << '\n';
    }
    for (size_t i = 0; i < values.length(); ++i) {
        WriteString(out, values.get(i), binary);
    }
    out.close();
    if (!out) {
        throw runtime_error("Ошибка записи");
    }
}
void ReadFile(const string& path, const string& kind, Array& values, bool binary) {
    ifstream in(path, ios::binary);
    if (!in) {
        throw runtime_error("Файл не найден");
    }
    string header;
    getline(in, header);
    string expected = "LR3TEXT1";
    if (binary) {
        expected = "LR3BIN1";
    }
    if (header != expected) {
        throw runtime_error("Неверный формат");
    }
    if (ReadString(in, binary) != kind) {
        throw runtime_error("Другой тип структуры");
    }
    uint32_t n;
    if (binary) {
        n = Get(in);
    } else {
        n = Number(in);
    }
    if (n > 100000) {
        throw runtime_error("Слишком много элементов");
    }
    Array temp;
    for (uint32_t i = 0; i < n; ++i) {
        temp.push_back(ReadString(in, binary));
    }
    if (in.peek() != EOF) {
        throw runtime_error("Лишние данные");
    }
    values.swap(temp);
}
void Storage::Values(const Array& o, Array& v) {
    for (size_t i = 0; i < o.length(); ++i) {
        v.push_back(o.get(i));
    }
}
void Storage::Restore(Array& o, const Array& v) {
    o.clear();
    for (size_t i = 0; i < v.length(); ++i) {
        o.push_back(v.get(i));
    }
}
void Storage::Values(const ForwardList& o, Array& v) {
    for (auto* n = o.head; n != nullptr; n = n->next) {
        v.push_back(n->value);
    }
}
void Storage::Restore(ForwardList& o, const Array& v) {
    o.clear();
    for (size_t i = 0; i < v.length(); ++i) {
        o.push_back(v.get(i));
    }
}
void Storage::Values(const List& o, Array& v) {
    for (auto* n = o.head; n != nullptr; n = n->next) {
        v.push_back(n->value);
    }
}
void Storage::Restore(List& o, const Array& v) {
    o.clear();
    for (size_t i = 0; i < v.length(); ++i) {
        o.push_back(v.get(i));
    }
}
void Storage::Values(const Stack& o, Array& v) {
    for (auto* n = o.head; n != nullptr; n = n->next) {
        v.push_back(n->value);
    }
}
void Storage::Restore(Stack& o, const Array& v) {
    o.clear();
    for (size_t i = v.length(); i > 0; --i) {
        o.push(v.get(i - 1));
    }
}
void Storage::Values(const Queue<string>& o, Array& v) {
    for (auto* n = o.head; n != nullptr; n = n->next) {
        v.push_back(n->value);
    }
}
void Storage::Restore(Queue<string>& o, const Array& v) {
    while (!o.empty()) {
        o.pop();
    }
    for (size_t i = 0; i < v.length(); ++i) {
        o.push(v.get(i));
    }
}
void Storage::Values(const HashTable& o, Array& v) {
    v.push_back(to_string(o.double_hash));
    Array names;
    o.names(names);
    for (size_t i = 0; i < names.length(); ++i) {
        string key = names.get(i), value;
        o.get(key, value);
        v.push_back(key);
        v.push_back(value);
    }
}
void Storage::Restore(HashTable& o, const Array& v) {
    if (v.length() == 0 || v.length() % 2 == 0 || (v.get(0) != "0" && v.get(0) != "1")) {
        throw runtime_error("Неверная таблица");
    }
    HashTable temp(v.get(0) == "1");
    for (size_t i = 1; i < v.length(); i += 2) {
        if (temp.has(v.get(i))) {
            throw runtime_error("Дубликат ключа");
        }
        temp.set(v.get(i), v.get(i + 1));
    }
    o.keys.swap(temp.keys);
    o.data.swap(temp.data);
    o.count = temp.count;
    o.used = temp.used;
    o.double_hash = temp.double_hash;
}
void Storage::Values(const Tree& o, Array& v) {
    Queue<const Tree::Node*> next;
    if (o.root) {
        next.push(o.root);
    }
    while (!next.empty()) {
        auto* n = next.pop();
        v.push_back(to_string(n->value));
        if (n->left) {
            next.push(n->left);
        }
        if (n->right) {
            next.push(n->right);
        }
    }
}
void Storage::Restore(Tree& o, const Array& v) {
    Tree temp;
    for (size_t i = 0; i < v.length(); ++i) {
        size_t used = 0;
        int n = stoi(v.get(i), &used);
        if (used != v.get(i).size()) {
            throw runtime_error("Неверное число дерева");
        }
        temp.insert(n);
    }
    auto* old = o.root;
    o.root = temp.root;
    temp.root = old;
}

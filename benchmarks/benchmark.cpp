#include "storage/storage.h"
#include <chrono>
#include <iostream>
using namespace std;
// Каждый замер создаёт, заполняет и уничтожает одну структуру из 1000 элементов.
// Повторяем пакет 10 раз, выводим среднее время одного пакета в микросекундах.
template <class F> void measure(const string& name, F action) {
    auto start = chrono::steady_clock::now();
    for (int i = 0; i < 10; ++i) {
        action();
    }
    auto finish = chrono::steady_clock::now();
    double time = chrono::duration<double, micro>(finish - start).count() / 10;
    cout << name << ": " << time << " us / 1000 elements\n";
}
int main() {
    measure("Array", [] {
        Array a;
        for (int i = 0; i < 1000; ++i) {
            a.push_back("value");
        }
    });
    measure("ForwardList", [] {
        ForwardList a;
        for (int i = 0; i < 1000; ++i) {
            a.push_back("value");
        }
    });
    measure("List", [] {
        List a;
        for (int i = 0; i < 1000; ++i) {
            a.push_back("value");
        }
    });
    measure("Queue", [] {
        Queue<string> a;
        for (int i = 0; i < 1000; ++i) {
            a.push("value");
        }
    });
    measure("Stack", [] {
        Stack a;
        for (int i = 0; i < 1000; ++i) {
            a.push("value");
        }
    });
    measure("HashTable", [] {
        HashTable a;
        for (int i = 0; i < 1000; ++i) {
            a.set(to_string(i), "value");
        }
    });
    measure("Tree", [] {
        Tree a;
        for (int i = 0; i < 1000; ++i) {
            a.insert(i);
        }
    });
}

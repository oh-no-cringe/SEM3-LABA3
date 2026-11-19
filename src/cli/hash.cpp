#include "hash/hash_table.h"
#include "commands.h"
#include "input.h"
#include <iostream>
#include <stdexcept>

using namespace std;

void run_hash() {
    cout << "Выбери метод: 1 — линейное пробирование, 2 — двойное хеширование\n";
    int method = read_number();
    if (method != 1 && method != 2) {
        throw runtime_error("Нужен метод 1 или 2");
    }
    HashTable table(method == 2);
    cout << "SET ключ значение | GET ключ | HAS ключ | DEL ключ | PRINT | EXIT\n";
    string command;
    while (cin >> command) {
        if (command == "EXIT") {
            return;
        } else if (command == "PRINT") {
            table.print();
        } else if (command == "SET") {
            string key = read_word();
            string value = read_word();
            table.set(key, value);
            cout << "OK\n";
        } else if (command == "GET") {
            string key = read_word();
            string value;
            if (table.get(key, value)) {
                cout << value << '\n';
            } else {
                cout << "NOT_FOUND\n";
            }
        } else if (command == "HAS") {
            print_bool(table.has(read_word()));
        } else if (command == "DEL") {
            table.del(read_word());
            cout << "OK\n";
        } else {
            throw runtime_error("Неизвестная команда хеш-таблицы");
        }
    }
}

#include "storage/storage.h"
#include "array/array.h"
#include "commands.h"
#include "input.h"
#include <iostream>
#include <stdexcept>

using namespace std;

void run_array() {
    Array array;
    cout << "PUSH значение | INSERT индекс значение | GET индекс | SET индекс значение\n"
         << "DEL индекс | SIZE | EMPTY | PRINT | EXIT\n";
    string command;
    while (cin >> command) {
        if (command == "SAVE" || command == "LOAD") {
            string format = read_word();
            string path = read_word();
            if (format != "text" && format != "binary") {
                throw runtime_error("Формат: text или binary");
            }
            if (command == "SAVE") {
                Save(array, path, "array", format == "binary");
            } else {
                Load(array, path, "array", format == "binary");
            }
            cout << "OK\n";
            continue;
        }
        if (command == "EXIT") {
            return;
        } else if (command == "PUSH") {
            array.push_back(read_word());
        } else if (command == "INSERT" || command == "SET") {
            int index = read_index();
            string value = read_word();
            if (command == "INSERT") {
                array.push_index(value, index);
            } else {
                array.set(index, value);
            }
        } else if (command == "DEL") {
            array.pop_index(read_index());
        } else if (command == "GET") {
            cout << array.get(read_index()) << '\n';
            continue;
        } else if (command == "SIZE") {
            cout << array.length() << '\n';
            continue;
        } else if (command == "EMPTY") {
            print_bool(array.empty());
            continue;
        } else if (command != "PRINT") {
            throw runtime_error("Неизвестная команда массива");
        }
        array.print();
    }
}

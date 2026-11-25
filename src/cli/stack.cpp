#include "storage/storage.h"
#include "stack/stack.h"
#include "commands.h"
#include "input.h"
#include <iostream>
#include <stdexcept>

using namespace std;

void run_stack() {
    Stack queue;
    cout << "PUSH значение | POP | EMPTY | PRINT | EXIT\n";
    string command;
    while (cin >> command) {
        if (command == "SAVE" || command == "LOAD") {
            string format = read_word();
            string path = read_word();
            if (format != "text" && format != "binary") {
                throw runtime_error("Формат: text или binary");
            }
            if (command == "SAVE") {
                Save(queue, path, "stack", format == "binary");
            } else {
                Load(queue, path, "stack", format == "binary");
            }
            cout << "OK\n";
            continue;
        }
        if (command == "EXIT") {
            return;
        } else if (command == "PUSH") {
            queue.push(read_word());
        } else if (command == "POP") {
            if (queue.empty()) {
                cout << "EMPTY\n";
            } else {
                cout << queue.pop() << '\n';
            }
            continue;
        } else if (command == "EMPTY") {
            print_bool(queue.empty());
            continue;
        } else if (command != "PRINT") {
            throw runtime_error("Неизвестная команда стека или очереди");
        }
        queue.print();
    }
}

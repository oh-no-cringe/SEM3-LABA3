#ifndef LR3_LIST_COMMANDS_H
#define LR3_LIST_COMMANDS_H

#include "input.h"
#include <iostream>
#include <stdexcept>

template <class T> static void run_list(T& list) {
    std::cout
        << "PUSH значение | FRONT значение | INSERT индекс значение | DEL индекс\n"
        << "DEL_VALUE значение | FIND значение | POP_FRONT | POP_BACK | EMPTY | PRINT | EXIT\n";
    std::string command;
    while (std::cin >> command) {
        if (command == "EXIT") {
            return;
        } else if (command == "PUSH") {
            list.push_back(read_word());
        } else if (command == "FRONT") {
            list.push_front(read_word());
        } else if (command == "INSERT") {
            int index = read_index();
            std::string value = read_word();
            list.push_index(value, index);
        } else if (command == "DEL") {
            list.pop_index(read_index());
        } else if (command == "DEL_VALUE") {
            list.erase_value(read_word());
        } else if (command == "POP_FRONT") {
            list.pop_front();
        } else if (command == "POP_BACK") {
            list.pop_back();
        } else if (command == "FIND") {
            print_bool(list.find(read_word()));
            continue;
        } else if (command == "EMPTY") {
            print_bool(list.empty());
            continue;
        } else if (command != "PRINT") {
            throw std::runtime_error("Неизвестная команда списка");
        }
        list.print();
    }
}

#endif

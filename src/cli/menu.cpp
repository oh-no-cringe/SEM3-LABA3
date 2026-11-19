#include "cli.h"
#include "commands.h"
#include <iostream>
#include <stdexcept>

using namespace std;

void print_menu() {
    cout << "Выбери структуру:\n"
         << "1 — массив\n"
         << "2 — односвязный список\n"
         << "3 — двусвязный список\n"
         << "4 — очередь\n"
         << "5 — стек\n"
         << "6 — хеш-таблица\n"
         << "7 — полное двоичное дерево\n";
}

void run_structure(int number) {
    if (number == 1) {
        run_array();
    } else if (number == 2) {
        run_forward();
    } else if (number == 3) {
        run_double_list();
    } else if (number == 4) {
        run_queue();
    } else if (number == 5) {
        run_stack();
    } else if (number == 6) {
        run_hash();
    } else if (number == 7) {
        run_tree();
    } else {
        throw runtime_error("Нужен номер от 1 до 7");
    }
}

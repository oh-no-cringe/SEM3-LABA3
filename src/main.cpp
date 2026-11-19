#include "cli/cli.h"
#include <exception>
#include <iostream>
#include <stdexcept>

using namespace std;

int main() {
    try {
        print_menu();
        int number;
        if (!(cin >> number)) {
            throw runtime_error("Нужен номер структуры");
        }
        run_structure(number);
        return 0;
    } catch (const exception& error) {
        cerr << "Ошибка: " << error.what() << '\n';
        return 1;
    }
}

#include "storage/storage.h"
#include "tree/tree.h"
#include "commands.h"
#include "input.h"
#include <iostream>
#include <stdexcept>

using namespace std;

void run_tree() {
    Tree tree;
    cout << "INSERT число | FIND число | CHECK | PRE | IN | POST | EMPTY | PRINT | EXIT\n";
    string command;
    while (cin >> command) {
        if (command == "SAVE" || command == "LOAD") {
            string format = read_word();
            string path = read_word();
            if (format != "text" && format != "binary") {
                throw runtime_error("Формат: text или binary");
            }
            if (command == "SAVE") {
                Save(tree, path, "tree", format == "binary");
            } else {
                Load(tree, path, "tree", format == "binary");
            }
            cout << "OK\n";
            continue;
        }
        if (command == "EXIT") {
            return;
        } else if (command == "INSERT") {
            tree.insert(read_number());
            tree.print();
        } else if (command == "FIND") {
            print_bool(tree.search(read_number()));
        } else if (command == "CHECK") {
            print_bool(tree.is_complete());
        } else if (command == "PRE") {
            tree.print_preorder();
        } else if (command == "IN") {
            tree.print_inorder();
        } else if (command == "POST") {
            tree.print_postorder();
        } else if (command == "EMPTY") {
            print_bool(tree.empty());
        } else if (command == "PRINT") {
            tree.print();
        } else {
            throw runtime_error("Неизвестная команда дерева");
        }
    }
}

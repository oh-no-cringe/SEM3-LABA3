#include "tree/tree.h"
#include <catch_amalgamated.hpp>
#include <iostream>
#include <sstream>

using namespace std;

TEST_CASE("TreeTest.StartsEmpty") {
    Tree tree;
    CHECK(tree.empty());
    CHECK(tree.is_complete());
    CHECK_FALSE(tree.search(10));
}

TEST_CASE("TreeTest.InsertsAndFindsValues") {
    Tree tree;
    for (int value : {10, -20, 30, 10}) {
        tree.insert(value);
        CHECK(tree.is_complete());
    }
    CHECK_FALSE(tree.empty());
    CHECK(tree.search(10));
    CHECK(tree.search(-20));
    CHECK(tree.search(30));
    CHECK_FALSE(tree.search(99));
}

TEST_CASE("TreeTest.PrintsLevelsAndDepthTraversals") {
    Tree tree;
    for (int value : {10, 20, 30, 40}) {
        tree.insert(value);
    }
    ostringstream output;
    streambuf* old = cout.rdbuf(output.rdbuf());
    tree.print();
    tree.print_preorder();
    tree.print_inorder();
    tree.print_postorder();
    cout.rdbuf(old);
    CHECK((output.str()) == ("[10, 20, 30, 40]\n"
                             "[10, 20, 40, 30]\n"
                             "[40, 20, 10, 30]\n"
                             "[40, 20, 30, 10]\n"));
}

TEST_CASE("TreeTest.KeepsCompleteShapeWhileGrowing") {
    Tree tree;
    for (int i = 0; i < 100; ++i) {
        tree.insert(i);
        CHECK(tree.is_complete());
    }
    for (int i = 0; i < 100; ++i) {
        CHECK(tree.search(i));
    }
}

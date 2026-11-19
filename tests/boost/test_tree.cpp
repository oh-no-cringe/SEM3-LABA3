#include "tree/tree.h"
#include <boost/test/unit_test.hpp>
#include <iostream>
#include <sstream>

using namespace std;

BOOST_AUTO_TEST_SUITE(TreeTest)

BOOST_AUTO_TEST_CASE(StartsEmpty) {
    Tree tree;
    BOOST_CHECK(tree.empty());
    BOOST_CHECK(tree.is_complete());
    BOOST_CHECK(!(tree.search(10)));
}

BOOST_AUTO_TEST_CASE(InsertsAndFindsValues) {
    Tree tree;
    for (int value : {10, -20, 30, 10}) {
        tree.insert(value);
        BOOST_CHECK(tree.is_complete());
    }
    BOOST_CHECK(!(tree.empty()));
    BOOST_CHECK(tree.search(10));
    BOOST_CHECK(tree.search(-20));
    BOOST_CHECK(tree.search(30));
    BOOST_CHECK(!(tree.search(99)));
}

BOOST_AUTO_TEST_CASE(PrintsLevelsAndDepthTraversals) {
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
    BOOST_CHECK_EQUAL(output.str(), "[10, 20, 30, 40]\n"
                                    "[10, 20, 40, 30]\n"
                                    "[40, 20, 10, 30]\n"
                                    "[40, 20, 30, 10]\n");
}

BOOST_AUTO_TEST_CASE(KeepsCompleteShapeWhileGrowing) {
    Tree tree;
    for (int i = 0; i < 100; ++i) {
        tree.insert(i);
        BOOST_CHECK(tree.is_complete());
    }
    for (int i = 0; i < 100; ++i) {
        BOOST_CHECK(tree.search(i));
    }
}

BOOST_AUTO_TEST_SUITE_END()

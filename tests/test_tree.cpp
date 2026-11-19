#include "tree/tree.h"
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>

using namespace std;

TEST(TreeTest, StartsEmpty) {
    Tree tree;
    EXPECT_TRUE(tree.empty());
    EXPECT_TRUE(tree.is_complete());
    EXPECT_FALSE(tree.search(10));
}

TEST(TreeTest, InsertsAndFindsValues) {
    Tree tree;
    for (int value : {10, -20, 30, 10}) {
        tree.insert(value);
        EXPECT_TRUE(tree.is_complete());
    }
    EXPECT_FALSE(tree.empty());
    EXPECT_TRUE(tree.search(10));
    EXPECT_TRUE(tree.search(-20));
    EXPECT_TRUE(tree.search(30));
    EXPECT_FALSE(tree.search(99));
}

TEST(TreeTest, PrintsLevelsAndDepthTraversals) {
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
    EXPECT_EQ(output.str(), "[10, 20, 30, 40]\n"
                            "[10, 20, 40, 30]\n"
                            "[40, 20, 10, 30]\n"
                            "[40, 20, 30, 10]\n");
}

TEST(TreeTest, KeepsCompleteShapeWhileGrowing) {
    Tree tree;
    for (int i = 0; i < 100; ++i) {
        tree.insert(i);
        EXPECT_TRUE(tree.is_complete());
    }
    for (int i = 0; i < 100; ++i) {
        EXPECT_TRUE(tree.search(i));
    }
}

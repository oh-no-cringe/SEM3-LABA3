#include "forward_list/forward_list.h"
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>

using namespace std;

TEST(ForwardListTest, StartsEmpty) {
    ForwardList list;
    EXPECT_TRUE(list.empty());
    EXPECT_FALSE(list.find("missing"));
    list.pop_front();
    list.pop_back();
    list.pop_index(0);
    EXPECT_TRUE(list.empty());
}

TEST(ForwardListTest, AddsInCorrectOrder) {
    ForwardList list;
    list.push_back("b");
    list.push_front("a");
    list.push_index("c", 2);
    ostringstream output;
    streambuf* old = cout.rdbuf(output.rdbuf());
    list.print();
    cout.rdbuf(old);
    EXPECT_EQ(output.str(), "[a, b, c]\n");
}

TEST(ForwardListTest, DeletesAndReusesList) {
    ForwardList list;
    list.push_back("a");
    list.push_back("b");
    list.push_back("c");
    list.pop_index(1);
    EXPECT_FALSE(list.find("b"));
    list.pop_front();
    list.pop_back();
    EXPECT_TRUE(list.empty());
    list.push_back("again");
    EXPECT_TRUE(list.find("again"));
    EXPECT_TRUE(list.erase_value("again"));
    EXPECT_TRUE(list.empty());
}

TEST(ForwardListTest, HandlesNeighborsAndMissingValues) {
    ForwardList list;
    list.push_back("b");
    EXPECT_TRUE(list.insert_before("b", "a"));
    EXPECT_TRUE(list.insert_after("b", "c"));
    EXPECT_TRUE(list.erase_before("b"));
    EXPECT_TRUE(list.erase_after("b"));
    EXPECT_FALSE(list.find("a"));
    EXPECT_FALSE(list.find("c"));
    EXPECT_TRUE(list.find("b"));
    EXPECT_FALSE(list.erase_before("b"));
    EXPECT_FALSE(list.erase_after("b"));
    EXPECT_FALSE(list.insert_before("missing", "x"));
    EXPECT_FALSE(list.insert_after("missing", "x"));
    EXPECT_FALSE(list.erase_value("missing"));
}

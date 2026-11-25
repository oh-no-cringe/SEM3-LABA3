#include "forward_list/forward_list.h"
#include <catch_amalgamated.hpp>
#include <iostream>
#include <sstream>

using namespace std;

TEST_CASE("ForwardListTest.StartsEmpty") {
    ForwardList list;
    CHECK(list.empty());
    CHECK_FALSE(list.find("missing"));
    list.pop_front();
    list.pop_back();
    list.pop_index(0);
    CHECK(list.empty());
}

TEST_CASE("ForwardListTest.AddsInCorrectOrder") {
    ForwardList list;
    list.push_back("b");
    list.push_front("a");
    list.push_index("c", 2);
    ostringstream output;
    streambuf* old = cout.rdbuf(output.rdbuf());
    list.print();
    cout.rdbuf(old);
    CHECK((output.str()) == ("[a, b, c]\n"));
}

TEST_CASE("ForwardListTest.DeletesAndReusesList") {
    ForwardList list;
    list.push_back("a");
    list.push_back("b");
    list.push_back("c");
    list.pop_index(1);
    CHECK_FALSE(list.find("b"));
    list.pop_front();
    list.pop_back();
    CHECK(list.empty());
    list.push_back("again");
    CHECK(list.find("again"));
    CHECK(list.erase_value("again"));
    CHECK(list.empty());
}

TEST_CASE("ForwardListTest.HandlesNeighborsAndMissingValues") {
    ForwardList list;
    list.push_back("b");
    CHECK(list.insert_before("b", "a"));
    CHECK(list.insert_after("b", "c"));
    CHECK(list.erase_before("b"));
    CHECK(list.erase_after("b"));
    CHECK_FALSE(list.find("a"));
    CHECK_FALSE(list.find("c"));
    CHECK(list.find("b"));
    CHECK_FALSE(list.erase_before("b"));
    CHECK_FALSE(list.erase_after("b"));
    CHECK_FALSE(list.insert_before("missing", "x"));
    CHECK_FALSE(list.insert_after("missing", "x"));
    CHECK_FALSE(list.erase_value("missing"));
}

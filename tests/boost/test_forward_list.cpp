#include "forward_list/forward_list.h"
#include <boost/test/unit_test.hpp>
#include <iostream>
#include <sstream>

using namespace std;

BOOST_AUTO_TEST_SUITE(ForwardListTest)

BOOST_AUTO_TEST_CASE(StartsEmpty) {
    ForwardList list;
    BOOST_CHECK(list.empty());
    BOOST_CHECK(!(list.find("missing")));
    list.pop_front();
    list.pop_back();
    list.pop_index(0);
    BOOST_CHECK(list.empty());
}

BOOST_AUTO_TEST_CASE(AddsInCorrectOrder) {
    ForwardList list;
    list.push_back("b");
    list.push_front("a");
    list.push_index("c", 2);
    ostringstream output;
    streambuf* old = cout.rdbuf(output.rdbuf());
    list.print();
    cout.rdbuf(old);
    BOOST_CHECK_EQUAL(output.str(), "[a, b, c]\n");
}

BOOST_AUTO_TEST_CASE(DeletesAndReusesList) {
    ForwardList list;
    list.push_back("a");
    list.push_back("b");
    list.push_back("c");
    list.pop_index(1);
    BOOST_CHECK(!(list.find("b")));
    list.pop_front();
    list.pop_back();
    BOOST_CHECK(list.empty());
    list.push_back("again");
    BOOST_CHECK(list.find("again"));
    BOOST_CHECK(list.erase_value("again"));
    BOOST_CHECK(list.empty());
}

BOOST_AUTO_TEST_CASE(HandlesNeighborsAndMissingValues) {
    ForwardList list;
    list.push_back("b");
    BOOST_CHECK(list.insert_before("b", "a"));
    BOOST_CHECK(list.insert_after("b", "c"));
    BOOST_CHECK(list.erase_before("b"));
    BOOST_CHECK(list.erase_after("b"));
    BOOST_CHECK(!(list.find("a")));
    BOOST_CHECK(!(list.find("c")));
    BOOST_CHECK(list.find("b"));
    BOOST_CHECK(!(list.erase_before("b")));
    BOOST_CHECK(!(list.erase_after("b")));
    BOOST_CHECK(!(list.insert_before("missing", "x")));
    BOOST_CHECK(!(list.insert_after("missing", "x")));
    BOOST_CHECK(!(list.erase_value("missing")));
}

BOOST_AUTO_TEST_SUITE_END()

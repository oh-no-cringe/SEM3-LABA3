#include "stack/stack.h"
#include <boost/test/unit_test.hpp>
#include <stdexcept>

using namespace std;

BOOST_AUTO_TEST_SUITE(StackTest)

BOOST_AUTO_TEST_CASE(StartsEmpty) {
    Stack stack;
    BOOST_CHECK(stack.empty());
    BOOST_CHECK_THROW(stack.pop(), out_of_range);
}

BOOST_AUTO_TEST_CASE(LastAddedComesFirst) {
    Stack stack;
    stack.push("apple");
    stack.push("pear");
    BOOST_CHECK_EQUAL(stack.pop(), "pear");
    BOOST_CHECK_EQUAL(stack.pop(), "apple");
    BOOST_CHECK(stack.empty());
}

BOOST_AUTO_TEST_CASE(ReusesAfterEmptying) {
    Stack stack;
    stack.push("first");
    BOOST_CHECK_EQUAL(stack.pop(), "first");
    stack.push("");
    BOOST_CHECK(!(stack.empty()));
    BOOST_CHECK_EQUAL(stack.pop(), "");
    BOOST_CHECK(stack.empty());
}

BOOST_AUTO_TEST_SUITE_END()

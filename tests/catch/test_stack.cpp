#include "stack/stack.h"
#include <catch_amalgamated.hpp>
#include <stdexcept>

using namespace std;

TEST_CASE("StackTest.StartsEmpty") {
    Stack stack;
    CHECK(stack.empty());
    CHECK_THROWS_AS(stack.pop(), out_of_range);
}

TEST_CASE("StackTest.LastAddedComesFirst") {
    Stack stack;
    stack.push("apple");
    stack.push("pear");
    CHECK((stack.pop()) == ("pear"));
    CHECK((stack.pop()) == ("apple"));
    CHECK(stack.empty());
}

TEST_CASE("StackTest.ReusesAfterEmptying") {
    Stack stack;
    stack.push("first");
    CHECK((stack.pop()) == ("first"));
    stack.push("");
    CHECK_FALSE(stack.empty());
    CHECK((stack.pop()) == (""));
    CHECK(stack.empty());
}

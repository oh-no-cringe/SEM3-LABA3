#include "stack/stack.h"
#include <gtest/gtest.h>
#include <stdexcept>

using namespace std;

TEST(StackTest, StartsEmpty) {
    Stack stack;
    EXPECT_TRUE(stack.empty());
    EXPECT_THROW(stack.pop(), out_of_range);
}

TEST(StackTest, LastAddedComesFirst) {
    Stack stack;
    stack.push("apple");
    stack.push("pear");
    EXPECT_EQ(stack.pop(), "pear");
    EXPECT_EQ(stack.pop(), "apple");
    EXPECT_TRUE(stack.empty());
}

TEST(StackTest, ReusesAfterEmptying) {
    Stack stack;
    stack.push("first");
    EXPECT_EQ(stack.pop(), "first");
    stack.push("");
    EXPECT_FALSE(stack.empty());
    EXPECT_EQ(stack.pop(), "");
    EXPECT_TRUE(stack.empty());
}

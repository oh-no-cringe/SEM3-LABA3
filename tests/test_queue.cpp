#include "queue/queue.h"
#include <gtest/gtest.h>
#include <stdexcept>
#include <string>

using namespace std;

TEST(QueueTest, StartsEmpty) {
    Queue<string> queue;
    EXPECT_TRUE(queue.empty());
    EXPECT_THROW(queue.pop(), out_of_range);
}

TEST(QueueTest, FirstAddedComesFirst) {
    Queue<string> queue;
    queue.push("apple");
    queue.push("pear");
    EXPECT_EQ(queue.pop(), "apple");
    EXPECT_EQ(queue.pop(), "pear");
    EXPECT_TRUE(queue.empty());
}

TEST(QueueTest, ReusesAfterEmptying) {
    Queue<string> queue;
    queue.push("first");
    EXPECT_EQ(queue.pop(), "first");
    queue.push("");
    EXPECT_EQ(queue.pop(), "");
    EXPECT_TRUE(queue.empty());
}

TEST(QueueTest, StoresPointers) {
    int value = 10;
    {
        Queue<int*> queue;
        queue.push(&value);
        EXPECT_EQ(queue.pop(), &value);
        queue.push(nullptr);
        EXPECT_EQ(queue.pop(), nullptr);
        queue.push(&value);
    }
    EXPECT_EQ(value, 10);
}

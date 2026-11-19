#include "array/array.h"
#include <gtest/gtest.h>
#include <stdexcept>

using namespace std;

TEST(ArrayTest, StartsEmpty) {
    Array array;
    EXPECT_TRUE(array.empty());
    EXPECT_EQ(array.length(), 0U);
}

TEST(ArrayTest, GrowsAndKeepsValues) {
    Array array;
    for (int i = 0; i < 100; ++i) {
        array.push_back(to_string(i));
    }
    ASSERT_EQ(array.length(), 100U);
    for (int i = 0; i < 100; ++i) {
        EXPECT_EQ(array.get(i), to_string(i));
    }
}

TEST(ArrayTest, InsertsChangesAndDeletes) {
    Array array;
    array.push_back("a");
    array.push_back("c");
    array.push_index("b", 1);
    ASSERT_EQ(array.length(), 3U);
    EXPECT_EQ(array.get(1), "b");
    array.set(1, "new");
    EXPECT_EQ(array.get(1), "new");
    array.pop_index(0);
    EXPECT_EQ(array.get(0), "new");
    EXPECT_EQ(array.get(1), "c");
    array.clear();
    EXPECT_TRUE(array.empty());
    array.push_back("");
    EXPECT_EQ(array.get(0), "");
}

TEST(ArrayTest, ChecksIndexes) {
    Array array;
    EXPECT_THROW(array.get(0), out_of_range);
    EXPECT_THROW(array.set(0, "x"), out_of_range);
    EXPECT_THROW(array.push_index("x", 1), out_of_range);
    array.pop_index(0);
    EXPECT_TRUE(array.empty());
}

TEST(ArrayTest, SwapsContents) {
    Array first;
    Array second;
    first.push_back("a");
    second.push_back("b");
    second.push_back("c");
    first.swap(second);
    EXPECT_EQ(first.length(), 2U);
    EXPECT_EQ(first.get(0), "b");
    EXPECT_EQ(second.length(), 1U);
    EXPECT_EQ(second.get(0), "a");
}

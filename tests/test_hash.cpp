#include "hash/hash_table.h"
#include <gtest/gtest.h>
#include <functional>

using namespace std;

TEST(HashTest, FindsUpdatesAndDeletesInBothMethods) {
    for (bool use_double : {false, true}) {
        HashTable table(use_double);
        string value;
        EXPECT_FALSE(table.has("apple"));
        EXPECT_FALSE(table.get("apple", value));
        table.set("apple", "red");
        ASSERT_TRUE(table.get("apple", value));
        EXPECT_EQ(value, "red");
        table.set("apple", "green");
        ASSERT_TRUE(table.get("apple", value));
        EXPECT_EQ(value, "green");
        Array names;
        table.names(names);
        EXPECT_EQ(names.length(), 1U);
        table.del("apple");
        table.del("missing");
        EXPECT_FALSE(table.has("apple"));
    }
}

TEST(HashTest, GrowsAndKeepsRemainingKeys) {
    for (bool use_double : {false, true}) {
        HashTable table(use_double);
        for (int i = 0; i < 300; ++i) {
            table.set(to_string(i), "v" + to_string(i));
        }
        for (int i = 0; i < 300; i += 2) {
            table.del(to_string(i));
        }
        for (int i = 0; i < 300; ++i) {
            string value;
            EXPECT_EQ(table.get(to_string(i), value), i % 2 != 0);
            if (i % 2 != 0) {
                EXPECT_EQ(value, "v" + to_string(i));
            }
        }
        Array names;
        table.names(names);
        EXPECT_EQ(names.length(), 150U);
    }
}

TEST(HashTest, FindsKeysAfterDeletingCollision) {
    for (bool use_double : {false, true}) {
        HashTable table(use_double);
        Array keys;
        for (int i = 0; keys.length() < 4; ++i) {
            string key = "key_" + to_string(i);
            if (hash<string>{}(key) % 8 == 0) {
                keys.push_back(key);
                table.set(key, key);
            }
        }
        table.del(keys.get(0));
        for (size_t i = 1; i < keys.length(); ++i) {
            string value;
            EXPECT_TRUE(table.get(keys.get(i), value));
            EXPECT_EQ(value, keys.get(i));
        }
    }
}

TEST(HashTest, SupportsEmptyAndSpecialKeys) {
    for (bool use_double : {false, true}) {
        HashTable table(use_double);
        table.set("", "");
        table.set("-", "minus");
        table.set("+", "plus");
        string value;
        ASSERT_TRUE(table.get("", value));
        EXPECT_EQ(value, "");
        ASSERT_TRUE(table.get("-", value));
        EXPECT_EQ(value, "minus");
        ASSERT_TRUE(table.get("+", value));
        EXPECT_EQ(value, "plus");
    }
}

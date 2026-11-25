#include "hash/hash_table.h"
#include <catch_amalgamated.hpp>
#include <functional>

using namespace std;

TEST_CASE("HashTest.FindsUpdatesAndDeletesInBothMethods") {
    for (bool use_double : {false, true}) {
        HashTable table(use_double);
        string value;
        CHECK_FALSE(table.has("apple"));
        CHECK_FALSE(table.get("apple", value));
        table.set("apple", "red");
        REQUIRE(table.get("apple", value));
        CHECK((value) == ("red"));
        table.set("apple", "green");
        REQUIRE(table.get("apple", value));
        CHECK((value) == ("green"));
        Array names;
        table.names(names);
        CHECK((names.length()) == (1U));
        table.del("apple");
        table.del("missing");
        CHECK_FALSE(table.has("apple"));
    }
}

TEST_CASE("HashTest.GrowsAndKeepsRemainingKeys") {
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
            CHECK((table.get(to_string(i), value)) == (i % 2 != 0));
            if (i % 2 != 0) {
                CHECK((value) == ("v" + to_string(i)));
            }
        }
        Array names;
        table.names(names);
        CHECK((names.length()) == (150U));
    }
}

TEST_CASE("HashTest.FindsKeysAfterDeletingCollision") {
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
            CHECK(table.get(keys.get(i), value));
            CHECK((value) == (keys.get(i)));
        }
    }
}

TEST_CASE("HashTest.SupportsEmptyAndSpecialKeys") {
    for (bool use_double : {false, true}) {
        HashTable table(use_double);
        table.set("", "");
        table.set("-", "minus");
        table.set("+", "plus");
        string value;
        REQUIRE(table.get("", value));
        CHECK((value) == (""));
        REQUIRE(table.get("-", value));
        CHECK((value) == ("minus"));
        REQUIRE(table.get("+", value));
        CHECK((value) == ("plus"));
    }
}

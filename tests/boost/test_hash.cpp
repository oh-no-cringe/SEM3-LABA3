#include "hash/hash_table.h"
#include <boost/test/unit_test.hpp>
#include <functional>

using namespace std;

BOOST_AUTO_TEST_SUITE(HashTest)

BOOST_AUTO_TEST_CASE(FindsUpdatesAndDeletesInBothMethods) {
    for (bool use_double : {false, true}) {
        HashTable table(use_double);
        string value;
        BOOST_CHECK(!(table.has("apple")));
        BOOST_CHECK(!(table.get("apple", value)));
        table.set("apple", "red");
        BOOST_REQUIRE(table.get("apple", value));
        BOOST_CHECK_EQUAL(value, "red");
        table.set("apple", "green");
        BOOST_REQUIRE(table.get("apple", value));
        BOOST_CHECK_EQUAL(value, "green");
        Array names;
        table.names(names);
        BOOST_CHECK_EQUAL(names.length(), 1U);
        table.del("apple");
        table.del("missing");
        BOOST_CHECK(!(table.has("apple")));
    }
}

BOOST_AUTO_TEST_CASE(GrowsAndKeepsRemainingKeys) {
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
            BOOST_CHECK_EQUAL(table.get(to_string(i), value), i % 2 != 0);
            if (i % 2 != 0) {
                BOOST_CHECK_EQUAL(value, "v" + to_string(i));
            }
        }
        Array names;
        table.names(names);
        BOOST_CHECK_EQUAL(names.length(), 150U);
    }
}

BOOST_AUTO_TEST_CASE(FindsKeysAfterDeletingCollision) {
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
            BOOST_CHECK(table.get(keys.get(i), value));
            BOOST_CHECK_EQUAL(value, keys.get(i));
        }
    }
}

BOOST_AUTO_TEST_CASE(SupportsEmptyAndSpecialKeys) {
    for (bool use_double : {false, true}) {
        HashTable table(use_double);
        table.set("", "");
        table.set("-", "minus");
        table.set("+", "plus");
        string value;
        BOOST_REQUIRE(table.get("", value));
        BOOST_CHECK_EQUAL(value, "");
        BOOST_REQUIRE(table.get("-", value));
        BOOST_CHECK_EQUAL(value, "minus");
        BOOST_REQUIRE(table.get("+", value));
        BOOST_CHECK_EQUAL(value, "plus");
    }
}

BOOST_AUTO_TEST_SUITE_END()

#include "array/array.h"
#include <catch_amalgamated.hpp>
#include <stdexcept>

using namespace std;

TEST_CASE("ArrayTest.StartsEmpty") {
    Array array;
    CHECK(array.empty());
    CHECK((array.length()) == (0U));
}

TEST_CASE("ArrayTest.GrowsAndKeepsValues") {
    Array array;
    for (int i = 0; i < 100; ++i) {
        array.push_back(to_string(i));
    }
    REQUIRE((array.length()) == (100U));
    for (int i = 0; i < 100; ++i) {
        CHECK((array.get(i)) == (to_string(i)));
    }
}

TEST_CASE("ArrayTest.InsertsChangesAndDeletes") {
    Array array;
    array.push_back("a");
    array.push_back("c");
    array.push_index("b", 1);
    REQUIRE((array.length()) == (3U));
    CHECK((array.get(1)) == ("b"));
    array.set(1, "new");
    CHECK((array.get(1)) == ("new"));
    array.pop_index(0);
    CHECK((array.get(0)) == ("new"));
    CHECK((array.get(1)) == ("c"));
    array.clear();
    CHECK(array.empty());
    array.push_back("");
    CHECK((array.get(0)) == (""));
}

TEST_CASE("ArrayTest.ChecksIndexes") {
    Array array;
    CHECK_THROWS_AS(array.get(0), out_of_range);
    CHECK_THROWS_AS(array.set(0, "x"), out_of_range);
    CHECK_THROWS_AS(array.push_index("x", 1), out_of_range);
    array.pop_index(0);
    CHECK(array.empty());
}

TEST_CASE("ArrayTest.SwapsContents") {
    Array first;
    Array second;
    first.push_back("a");
    second.push_back("b");
    second.push_back("c");
    first.swap(second);
    CHECK((first.length()) == (2U));
    CHECK((first.get(0)) == ("b"));
    CHECK((second.length()) == (1U));
    CHECK((second.get(0)) == ("a"));
}

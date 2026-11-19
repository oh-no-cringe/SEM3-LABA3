#include "array/array.h"
#include <boost/test/unit_test.hpp>
#include <stdexcept>

using namespace std;

BOOST_AUTO_TEST_SUITE(ArrayTest)

BOOST_AUTO_TEST_CASE(StartsEmpty) {
    Array array;
    BOOST_CHECK(array.empty());
    BOOST_CHECK_EQUAL(array.length(), 0U);
}

BOOST_AUTO_TEST_CASE(GrowsAndKeepsValues) {
    Array array;
    for (int i = 0; i < 100; ++i) {
        array.push_back(to_string(i));
    }
    BOOST_REQUIRE_EQUAL(array.length(), 100U);
    for (int i = 0; i < 100; ++i) {
        BOOST_CHECK_EQUAL(array.get(i), to_string(i));
    }
}

BOOST_AUTO_TEST_CASE(InsertsChangesAndDeletes) {
    Array array;
    array.push_back("a");
    array.push_back("c");
    array.push_index("b", 1);
    BOOST_REQUIRE_EQUAL(array.length(), 3U);
    BOOST_CHECK_EQUAL(array.get(1), "b");
    array.set(1, "new");
    BOOST_CHECK_EQUAL(array.get(1), "new");
    array.pop_index(0);
    BOOST_CHECK_EQUAL(array.get(0), "new");
    BOOST_CHECK_EQUAL(array.get(1), "c");
    array.clear();
    BOOST_CHECK(array.empty());
    array.push_back("");
    BOOST_CHECK_EQUAL(array.get(0), "");
}

BOOST_AUTO_TEST_CASE(ChecksIndexes) {
    Array array;
    BOOST_CHECK_THROW(array.get(0), out_of_range);
    BOOST_CHECK_THROW(array.set(0, "x"), out_of_range);
    BOOST_CHECK_THROW(array.push_index("x", 1), out_of_range);
    array.pop_index(0);
    BOOST_CHECK(array.empty());
}

BOOST_AUTO_TEST_CASE(SwapsContents) {
    Array first;
    Array second;
    first.push_back("a");
    second.push_back("b");
    second.push_back("c");
    first.swap(second);
    BOOST_CHECK_EQUAL(first.length(), 2U);
    BOOST_CHECK_EQUAL(first.get(0), "b");
    BOOST_CHECK_EQUAL(second.length(), 1U);
    BOOST_CHECK_EQUAL(second.get(0), "a");
}

BOOST_AUTO_TEST_SUITE_END()

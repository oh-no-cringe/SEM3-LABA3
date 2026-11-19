#include "queue/queue.h"
#include <boost/test/unit_test.hpp>
#include <stdexcept>
#include <string>

using namespace std;

BOOST_AUTO_TEST_SUITE(QueueTest)

BOOST_AUTO_TEST_CASE(StartsEmpty) {
    Queue<string> queue;
    BOOST_CHECK(queue.empty());
    BOOST_CHECK_THROW(queue.pop(), out_of_range);
}

BOOST_AUTO_TEST_CASE(FirstAddedComesFirst) {
    Queue<string> queue;
    queue.push("apple");
    queue.push("pear");
    BOOST_CHECK_EQUAL(queue.pop(), "apple");
    BOOST_CHECK_EQUAL(queue.pop(), "pear");
    BOOST_CHECK(queue.empty());
}

BOOST_AUTO_TEST_CASE(ReusesAfterEmptying) {
    Queue<string> queue;
    queue.push("first");
    BOOST_CHECK_EQUAL(queue.pop(), "first");
    queue.push("");
    BOOST_CHECK_EQUAL(queue.pop(), "");
    BOOST_CHECK(queue.empty());
}

BOOST_AUTO_TEST_CASE(StoresPointers) {
    int value = 10;
    {
        Queue<int*> queue;
        queue.push(&value);
        BOOST_CHECK_EQUAL(queue.pop(), &value);
        queue.push(nullptr);
        BOOST_CHECK_EQUAL(queue.pop(), nullptr);
        queue.push(&value);
    }
    BOOST_CHECK_EQUAL(value, 10);
}

BOOST_AUTO_TEST_SUITE_END()

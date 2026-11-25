#include "queue/queue.h"
#include <catch_amalgamated.hpp>
#include <stdexcept>
#include <string>

using namespace std;

TEST_CASE("QueueTest.StartsEmpty") {
    Queue<string> queue;
    CHECK(queue.empty());
    CHECK_THROWS_AS(queue.pop(), out_of_range);
}

TEST_CASE("QueueTest.FirstAddedComesFirst") {
    Queue<string> queue;
    queue.push("apple");
    queue.push("pear");
    CHECK((queue.pop()) == ("apple"));
    CHECK((queue.pop()) == ("pear"));
    CHECK(queue.empty());
}

TEST_CASE("QueueTest.ReusesAfterEmptying") {
    Queue<string> queue;
    queue.push("first");
    CHECK((queue.pop()) == ("first"));
    queue.push("");
    CHECK((queue.pop()) == (""));
    CHECK(queue.empty());
}

TEST_CASE("QueueTest.StoresPointers") {
    int value = 10;
    {
        Queue<int*> queue;
        queue.push(&value);
        CHECK((queue.pop()) == (&value));
        queue.push(nullptr);
        CHECK((queue.pop()) == (nullptr));
        queue.push(&value);
    }
    CHECK((value) == (10));
}

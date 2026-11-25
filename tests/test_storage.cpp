#include "storage/storage.h"
#include <gtest/gtest.h>
#include <fstream>
#include <cstdio>
using namespace std;
template <class T> void CheckFile(T& first, T& second, string kind) {
    for (bool binary : {false, true}) {
        string path = "build/storage-test-" + kind + to_string(binary);
        Save(first, path, kind, binary);
        Load(second, path, kind, binary);
        Array a, b;
        Storage::Values(first, a);
        Storage::Values(second, b);
        ASSERT_EQ(a.length(), b.length());
        for (size_t i = 0; i < a.length(); ++i) {
            EXPECT_EQ(a.get(i), b.get(i));
        }
        ofstream bad(path, ios::binary | ios::trunc);
        bad << "BAD";
        bad.close();
        EXPECT_THROW(Load(second, path, kind, binary), exception);
        remove(path.c_str());
    }
}
TEST(StorageTest, AllStructuresAndFormats) {
    Array a, b;
    a.push_back("");
    a.push_back("строка\nс пробелами");
    a.push_back("quote\"\\");
    CheckFile(a, b, "array");
    ForwardList f, g;
    f.push_back("a");
    f.push_back("b");
    CheckFile(f, g, "list");
    List l, m;
    l.push_back("a");
    l.push_back("b");
    CheckFile(l, m, "list");
    Stack s, t;
    s.push("first");
    s.push("last");
    CheckFile(s, t, "stack");
    Queue<string> q, r;
    q.push("first");
    q.push("last");
    CheckFile(q, r, "queue");
    Tree x, y;
    for (int n : {10, -20, 30, 40}) {
        x.insert(n);
    }
    CheckFile(x, y, "tree");
    EXPECT_TRUE(y.is_complete());
    HashTable h(true), j;
    h.set("apple", "red");
    CheckFile(h, j, "hash");
}
TEST(StorageTest, EmptyAndDamagedFiles) {
    Array a, b;
    b.push_back("unchanged");
    CheckFile(a, b, "array");
    EXPECT_THROW(Load(a, "build/no-such-archive", "array", true), exception);
    EXPECT_THROW(Save(a, "build/no-directory/file", "array", false), exception);
    Save(a, "build/damaged.txt", "array", false);
    EXPECT_THROW(Load(a, "build/damaged.txt", "tree", false), exception);
    ofstream out("build/damaged.txt", ios::app);
    out << "extra";
    out.close();
    EXPECT_THROW(Load(a, "build/damaged.txt", "array", false), exception);
    remove("build/damaged.txt");
}

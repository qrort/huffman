#include <cassert>
#include <cstdlib>
#include <string>
#include <sstream>

#include "gtest/gtest.h"
#include "../header/tree-impl.h"

using namespace std;

void perform(stringstream &in, stringstream &tmp, stringstream &out) {
    huffman_tree cur;
    cur.make(in);

    in.clear();
    in.seekg(0);

    assert(in.tellg() == 0);

    cur.encode(in, tmp);
    cur.decode(tmp, out);
}

TEST(correctness, empty) {
    stringstream in("");
    stringstream tmp, out;

    perform(in, tmp, out);

    EXPECT_EQ(in.str(), out.str());
}


TEST(correctness, single_letter) {

    stringstream in("a");
    stringstream tmp, out;

    perform(in, tmp, out);

    EXPECT_EQ(in.str(), out.str());
}


TEST(correctness, single_letter_multiple) {

    stringstream in("aaaaaaaaaaaaaaaaaa");
    stringstream tmp, out;

    perform(in, tmp, out);

    EXPECT_EQ(in.str(), out.str());
}


TEST(correctness, sentence) {

    stringstream in("The quick brown fox jumps over a lazy dog");
    stringstream tmp, out;

    perform(in, tmp, out);

    EXPECT_EQ(in.str(), out.str());
}

TEST(correctness, random) {
    string input = "";
    for (int i = 0; i < 100000; i++) input.push_back(rand() % 256);

    stringstream in(input);
    stringstream tmp, out;

    perform(in, tmp, out);

    EXPECT_EQ(in.str(), out.str());
}

TEST(correctness, corrupted_tree) {
    stringstream encoded, decoded;
    encoded << "#$364";
    huffman_tree cur;
    bool ok = false;
    try {
        cur.read(encoded);
        cur.decode(encoded, decoded);
    } catch (...) {
       ok = true;
    }
    EXPECT_EQ(ok, true);
}


TEST(correctness, corrupted_text) {
    stringstream encoded, decoded;
    encoded << 3 << ' ' << 1 << ' ' << "011" << 'a';
    encoded << "#$364";
    huffman_tree cur;
    bool ok = false;
    try {
        cur.read(encoded);
        cur.decode(encoded, decoded);
    } catch (...) {
        ok = true;
    }
    EXPECT_EQ(ok, true);
}



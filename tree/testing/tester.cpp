#include <cassert>
#include <cstdlib>
#include <string>
#include <sstream>
#include <random>
#include <filesystem>
#include <chrono>

#include "gtest/gtest.h"
#include "archiver.h"
#include "bwt.h"
#include "mtf.h"
#include "tree-impl.h"

using namespace std;

using ustring = std::basic_string<unsigned char>;
namespace fs = std::filesystem;
const fs::path TMPFILE = fs::current_path() / "tmpfile";
const fs::path RESFILE = fs::current_path() / "resfile";

void archiver_file_test(const std::string &file) {
    chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    archiver::encode(file, TMPFILE);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cerr << "Time encoding: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;
    begin = std::chrono::steady_clock::now();
    archiver::decode(TMPFILE, RESFILE);
    end = std::chrono::steady_clock::now();
    std::cerr << "Time decoding: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;
    ifstream inf(file);
    ifstream rinf(RESFILE);
    ustring inp = archiver::read_stream(inf);
    ustring out = archiver::read_stream(rinf);
    cerr << "Initial file size: " << fs::file_size(file) << " bytes\n";
    cerr << "Result file size: " << fs::file_size(TMPFILE) << " bytes\n";
    fs::remove_all(TMPFILE);
    fs::remove_all(RESFILE);

    EXPECT_EQ(inp, out);
}

void perform(istream &in, iostream &tmp, iostream &out) {
    huffman_tree cur;
    cur.make(in);

    in.clear();
    in.seekg(0);

    assert(in.tellg() == 0);

    cur.encode(in, tmp);
    cur.decode(tmp, out);

    in.clear();
    in.seekg(0);
    tmp.clear();
    tmp.seekg(0);
    out.clear();
    out.seekg(0);

    EXPECT_EQ(archiver::read_stream(in), archiver::read_stream(out));
}


TEST(mtf, correct) {
    stringstream ss("BCABAAA");
    ustring s = archiver::read_stream(ss);
    EXPECT_EQ(s, mtf::reverse_mtf(mtf::mtf(s)));
}

TEST(bwt, correct) {
    string s = "oijqwpoeriunzxamcbfdkfdfdjhwefw";
    stringstream in("oijqwpoeriunzxamcbfdkfdfdjhwefw");
    bwt my_bwt(in);
    auto transformed = my_bwt.transform();
    auto restored = my_bwt.reverse_transform(transformed);
    for (int i = 0; i < s.size(); i++) {
        EXPECT_EQ(static_cast<unsigned char>(s[i]), restored[i]);
    }
}

TEST(bwt, unsigned_chars) {
    string s = "\255\123\100\0\1jab\n\254""\43""98";
    stringstream in("\255\123\100\0\1jab\n\254""\43""98");
    bwt my_bwt(in);
    auto transformed = my_bwt.transform();
    auto restored = my_bwt.reverse_transform(transformed);
    for (int i = 0; i < s.size(); i++) {
        EXPECT_EQ(static_cast<unsigned char>(s[i]), restored[i]);
    }
}

TEST(bwt, geo) {
    fs::path filename = fs::current_path() / "geo";
    ifstream in(filename);
    stringstream tmp, out;
    perform(in, tmp, out);
    ustring s = archiver::read_stream(in);
    s = s.substr(0, 100);
    bwt my_bwt(s);
    auto res = my_bwt.transform();
    EXPECT_EQ(s, my_bwt.reverse_transform(res));
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
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, 255);
    for (int i = 0; i < 100000; i++) input.push_back(dist(gen));

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

TEST(correctness, special_characters) {
    stringstream in("!_$$@@$##@*&&^^(!@*$+_=-=-+90*&");
    stringstream tmp, out;

    perform(in, tmp, out);

    EXPECT_EQ(in.str(), out.str());
}

std::uintmax_t file_size(ostream &stream) {
    std::uintmax_t res;
    try {
        fs::remove_all(TMPFILE);
    } catch(...) {
    }
    {
        std::ofstream outFile;
        outFile.open(TMPFILE);
        outFile << stream.rdbuf();
        outFile.close();
        res = fs::file_size(TMPFILE);
    }
    return res;
}

void corpus_test(const string &f) {
    fs::path filename = fs::current_path() / f;
    cerr << "Testing " << filename << '\n';
    ifstream in(filename);
    stringstream tmp, out;
    perform(in, tmp, out);
    ustring s = archiver::read_stream(in);
    bwt my_bwt(s);
    auto res = my_bwt.transform();
    EXPECT_EQ(s, my_bwt.reverse_transform(res));
    cerr << "Initial file size is " << fs::file_size(filename) << " bytes\n";
    cerr << "Compressed file size is " << file_size(tmp) << " bytes\n";
}

const vector<string> CALGARY_CORPUS = {
        "bib",
        "book1",
        "book2",
        "geo",
        "news",
        "obj1",
        "obj2",
        "paper1",
        "paper2",
        "pic",
        "progc",
        "progl",
        "progp",
        "trans"
};

TEST(performance, archiver_test) {
    for (const auto& file : CALGARY_CORPUS) {
        cerr << "HUFFMAN ONLY\n";
        corpus_test(file);
        cerr << "BWT + MTF + HUFFMAN\n";
        archiver_file_test(file);
    }
}
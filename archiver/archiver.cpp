#include "archiver.h"
#include "tree-impl.h"
#include "bwt.h"
#include "mtf.h"

#include <sstream>

using namespace std;

namespace archiver {
    ustring read_stream(istream& in) {
        ustring data;
        int const BUF_SIZE = (1 << 20);
        in >> std::noskipws;
        char buf[BUF_SIZE];
        while (in) {
            in.read(reinterpret_cast<char *>(buf), BUF_SIZE);
            size_t num = in.gcount();
            for (int i = 0; i < num; i++) {
                const unsigned char symbol = buf[i];
                data.push_back(symbol);
            }
        }
        in.clear();
        in.seekg(0);
        return data;
    }

    void encode(const std::string &inp, const std::string &target) {
        ifstream in;
        ofstream out;
        in.open(inp);
        out.open(target);

        if (!in.is_open() || !out.is_open()) throw runtime_error("Could not open files");

        ustring content = read_stream(in);

        bwt encode_bwt(content);
        auto res = encode_bwt.transform();
        auto fin = mtf::mtf(res);

        stringstream ss(std::string(reinterpret_cast<const char*>(fin.c_str()), fin.size()));
        huffman_tree tree;

        tree.make(ss);
        tree.write(out);

        ss = std::stringstream(std::string(reinterpret_cast<const char*>(fin.c_str()), fin.size()));

        tree.encode(ss, out);

        out.close();
    }

    void decode(const std::string &inp, const std::string &target) {
        ifstream in;
        ofstream out;

        stringstream huffman_out;

        in.open(inp);
        out.open(target);

        if (!in.is_open() || !out.is_open()) throw runtime_error("Could not open files");

        huffman_tree tree;
        tree.read(in);
        tree.decode(in, huffman_out);

        ustring dec = read_stream(huffman_out);
        ustring res = bwt::reverse_transform(mtf::reverse_mtf(dec));
        for (char c : res) {
            out.put(c);
        }
        in.close();
        out.close();
    }
}

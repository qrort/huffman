#ifndef HUFFMAN_ARCHIVER_H
#define HUFFMAN_ARCHIVER_H
#include <fstream>
#include <string>

namespace archiver {
    using ustring = std::basic_string<unsigned char>;

    void encode(const std::string &inp, const std::string &target);
    void decode(const std::string &inp, const std::string &target);
    ustring read_stream(std::istream& in);
};


#endif //HUFFMAN_ARCHIVER_H

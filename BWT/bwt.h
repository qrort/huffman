#ifndef HUFFMAN_BWT_H
#define HUFFMAN_BWT_H
#include <iostream>
#include <vector>
#include <string>

class bwt {
    using ustring = std::basic_string<unsigned char>;
public:
    explicit bwt(std::istream &in);
    explicit bwt(ustring s);
    ustring transform();
    static ustring reverse_transform(const ustring &coded) ;
    std::vector <int> build_suffix_array();
private:
    void radix_suffix_sort(int halfLen);
    std::vector <int> suffix_array;
    std::vector <int> c;
    static std::pair <ustring, int> split_coded_bwt(const ustring &coded);
    ustring data;
    size_t sz;
};

#endif //HUFFMAN_BWT_H

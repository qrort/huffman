#ifndef TREEIMPL
#define TREEIMPL
#include <iostream>
#include <vector>

struct huffman_tree{
private:
    std::vector <int> freq;
    struct vertex{
        bool terminal;
        unsigned char symbol;
        int l, r;
		vertex() {}
        vertex(bool terminal, unsigned char symbol, int l = -1, int r = -1) : terminal(terminal), symbol(symbol), l(l), r(r) {}
    };
    std::vector <vertex> data;
    int root;
    void dfs(std::vector <unsigned char> & output, int i, std::ostream& out, bool mode);
    void parse_vertexes(int i, std::vector<bool> & a, std::string & b, int & id1, int & id2);
    void get_codes(int i, std::vector <std::vector <bool>> & codes, std::vector <bool> & cur);
public:
    huffman_tree() : root(0), freq(256, 0) {}
    huffman_tree read(std::istream& in);
    huffman_tree make(std::istream& in);
    void write(std::ostream& out);
    void encode(std::istream&in, std::ostream& out);
    void decode(std::istream&in, std::ostream& out);
};

#endif

#include "../header/tree-impl.h"
#include <set>
#include <cassert>
#include <algorithm>

using namespace std;
int const BUF_SIZE = (1 << 20);

huffman_tree huffman_tree::make(istream& in) {
    unsigned char c, act = 'a';
    while(in >> noskipws >> c) {
        freq[c]++;
        act = c;
    }
    set <pair <int, int>> unused;
    for (int i = 0; i < 256; i++) if (freq[i]) {
        unused.insert({freq[i], data.size()});
        data.push_back(vertex(true, i));
    }
    while (unused.size() > 1) {
        auto a = *unused.begin();
        unused.erase(unused.begin());
        auto b = *unused.begin();
        unused.erase(unused.begin());
        unused.insert({a.first + b.first, data.size()});
        data.push_back(vertex(false, 0, a.second, b.second));
    }
    if (data.empty()) {
        data.push_back(vertex(true, act));
    }
    root = data.size() - 1;
    return *this;
}

void huffman_tree::parse_vertexes(int i, vector <bool> & edges, string & symbols, int & e, int & s) {
    data.push_back(vertex(false, 0));
    if (edges[e++]) {
        data[i] = vertex(true, symbols[s++]);
    } else {
        data[i].l = data.size();
        parse_vertexes(data.size(), edges, symbols, e, s);
        data[i].r = data.size();
        parse_vertexes(data.size(), edges, symbols, e, s);
    }
}

huffman_tree huffman_tree::read(istream& in) {//ok
    vector <bool> edges;
    string symbols = "";
    int sz, alph;
    in >> sz;
    if (!in) throw runtime_error("Encoded file is corrupted");
    in >> alph;
    if (!in) throw runtime_error("Encoded file is corrupted");
    unsigned char c;
    in >> noskipws >> c;
    if (!in) throw runtime_error("Encoded file is corrupted");
    if (c != ' ' || sz < 0 || alph < 0) throw runtime_error("Encoded file is corrupted");
    while (sz--) {
       in >> c;
       if (c != '1' && c != '0' || !in) throw runtime_error("Encoded file is corrupted");
       edges.push_back(c == '1');
    }
    while (alph--) {
        in >> c;
        if (!(0 <= c && c < 256) || (alph && !in)) throw runtime_error("Encoded file is corrupted");
        symbols.push_back(c);
    }
    root = 0;
    if (edges.empty() && !symbols.empty()) {
        data = { vertex(true, symbols[0]) };
    } else {
        int e, s;
        e = s = 0;
        parse_vertexes(0, edges, symbols, e, s);
    }
    return *this;
}

void huffman_tree::dfs(vector <unsigned char> & cur, int id, ostream& out, bool mode) {//ok
    vertex v = data[id];
    if (v.l != -1) {
        if (!mode) cur.push_back('0');
        dfs(cur, v.l, out, mode);
        dfs(cur, v.r, out, mode);
    } else {
        if (mode) cur.push_back((v.symbol)); else cur.push_back('1');
    }
}

void huffman_tree::write(ostream& out) {//ok
    vector <unsigned char> output;
    dfs(output, root, out, false);
    size_t tree_size = output.size();
    dfs(output, root, out, true);
    out << tree_size << ' ' << output.size() - tree_size << ' ';
    for (auto c : output) out << c;
}

void huffman_tree::get_codes(int i, vector <vector <bool>> & code, vector <bool> & cur) {
    if (data[i].terminal) {
        code[data[i].symbol] = cur;
        return;
    } else {
        cur.push_back(0);
        get_codes(data[i].l, code, cur);
        cur[cur.size() - 1] = 1;
        get_codes(data[i].r, code, cur);
        cur.pop_back();
    }
}


void huffman_tree::encode(istream& in, ostream& out) {//ok
    auto begin = out.tellp();
    out << ' ';
    vector <vector<bool>> code(256);
    vector <bool> mem;
    get_codes(root, code, mem);
    if (data.size() == 1) code[data[0].symbol] = {0};
    unsigned char buf[BUF_SIZE];
    char cur = 0; int cnt = 0;
    while (in) {
        in.read(reinterpret_cast<char*>(buf), BUF_SIZE);
        int sz = in.gcount();
        for (int i = 0; i < sz; i++) {
            const auto& seq = code[buf[i]];
            for (bool b : seq) {
                cur |= (b << cnt);
                cnt++;
                if (cnt == 8) {
                    out << cur;
                    cur = 0; cnt = 0;
                }
            }
        }
    }
    if (cnt) {
        out << cur;
        cnt = 8 - cnt;
    }
    out.seekp(begin);
    out << char(cnt);
}

void huffman_tree::decode(istream& in, ostream& out) {
    int v = root;
    char buf[BUF_SIZE];
    char extra;
    in >> noskipws >> extra;
    if (!in) throw runtime_error("Encoded file is corrupted");
    while(in) {
        in.read(reinterpret_cast<char*>(buf), BUF_SIZE);
        if (in.bad()) throw runtime_error("Encoded file is corrupted");
        size_t sz = in.gcount();
        for (size_t i = 0; i < sz; i++) {
            const unsigned char symbol = buf[i];
            for (size_t j = 0; j < 8 - (!in && i == sz - 1) * int(extra); j++) {
                bool c = static_cast<bool>(1 & (symbol >> j));
                if (data.size() > 1) if (c == 0) v = data[v].l; else v = data[v].r;
                if (v == -1 || j > 8) {
                    throw runtime_error("Encoded file is corrupted");
                }
                if (data[v].terminal) {
                    out << data[v].symbol;
                    v = root;
                }
            }
        }
    }
}

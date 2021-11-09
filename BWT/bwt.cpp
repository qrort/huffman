#include "bwt.h"
#include <algorithm>
#include <string>

using ustring = std::basic_string<unsigned char>;

bwt::bwt(std::istream &in) {
    int const BUF_SIZE = (1 << 20);
    in >> std::noskipws;
    char buf[BUF_SIZE];
    while(in) {
        in.read(reinterpret_cast<char *>(buf), BUF_SIZE);
        size_t num = in.gcount();
        for (int i = 0; i < num; i++) {
            const unsigned char symbol = buf[i];
            data.push_back(symbol);
        }
    }
    in.clear();
    in.seekg(0);
    sz = data.size();
}

bwt::bwt(ustring tmp) : data(std::move(tmp)), sz(data.size()) {}

void bwt::radix_suffix_sort(int halfLen) {
    auto next = [this, halfLen](int i) {
        return (i + halfLen) % sz;
    };
    std::vector <int> pn(sz);
    for (int i = 0; i < sz; i++) {
        pn[i] = next(suffix_array[i]);
    }
    int const ALPH = sz;
    std::vector <int> cnt(ALPH);
    for (int i = 0; i < sz; i++) {
        cnt[c[i]]++;
    }
    for (int i = 1; i < ALPH; i++) {
        cnt[i] += cnt[i - 1];
    }
    auto cnt_copy = cnt;
    std::vector <int> perm(sz);
    for (int i = sz - 1; i >= 0; i--) {
        perm[--cnt[c[pn[i]]]] = suffix_array[i];
    }
    swap(cnt, cnt_copy);
    for (int i = sz - 1; i >= 0; i--) {
        suffix_array[--cnt[c[perm[i]]]] = perm[i];
    }
}

std::vector <int> bwt::build_suffix_array() {
    {
        int id = 0;
        std::generate_n(std::back_inserter(suffix_array), sz, [&id]() mutable {
            return id++;
        });
    }
    std::sort(suffix_array.begin(), suffix_array.end(), [this] (int i, int j) {
        return data[i] < data[j];
    });
    c.resize(sz, 0);
    for (int i = 1; i < suffix_array.size(); i++) {
        unsigned char ch = data[suffix_array[i]];
        if (ch != data[suffix_array[i - 1]]) {
            c[suffix_array[i]] = c[suffix_array[i - 1]] + 1;
        } else {
            c[suffix_array[i]] = c[suffix_array[i - 1]];
        }
    }

    for (int halfLen = 1; halfLen < sz; halfLen *= 2) {
        auto next = [this, halfLen](int i) {
            return (i + halfLen) % sz;
        };
        radix_suffix_sort(halfLen);
        std::vector <int> cn(sz);
        const auto& suf = suffix_array;
        for (int i = 1; i < sz; i++) {
            if (c[suf[i]] != c[suf[i - 1]] || c[next(suf[i])] != c[next(suf[i - 1])]) {
                cn[suf[i]] = cn[suf[i - 1]] + 1;
            } else {
                cn[suf[i]] = cn[suf[i - 1]];
            }
        }
        swap(c, cn);
    }
    return suffix_array;
}

ustring bwt::transform() {
    auto suf = build_suffix_array();
    ustring result;
    result.resize(sz);
    int saved = -1;
    for (int i = 0; i < sz; i++) {
        if (!suf[i]) {
            saved = i;
        }
        result[i] = data[(suf[i] - 1 + sz) % sz];
    }
    std::string tmp = std::to_string(saved);
    ustring final;
    for (char i : tmp) {
        final.push_back(i);
    }
    final.push_back('\n');
    final += result;
    return final;
}

std::pair<ustring, int> bwt::split_coded_bwt(const ustring &coded) {
    std::string num;
    int i = 0;
    for (; coded[i] != '\n'; i++) {
        num.push_back(coded[i]);
    }
    i++;
    return {coded.substr(i, coded.size() - i), std::stoi(num)};
}

ustring bwt::reverse_transform(const ustring &str) {
    auto coded = split_coded_bwt(str);
    int const ALPH = 256;
    const ustring &s = coded.first;
    std::vector <int> cnt(ALPH);
    for (unsigned char ch : s) {
        cnt[ch]++;
    }
    int sum = 0;
    for (int i = 0; i < ALPH; i++) {
        sum += cnt[i];
        cnt[i] = sum - cnt[i];
    }
    std::vector <int> perm(s.size());
    for (int i = 0; i < s.size(); i++) {
        perm[cnt[s[i]]] = i;
        cnt[s[i]]++;
    }
    int j = perm[coded.second];
    ustring res;
    res.resize(s.size());
    for (int i = 0; i < s.size(); i++) {
        res[i] = s[j];
        j = perm[j];
    }
    return res;
}

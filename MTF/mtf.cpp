#include "mtf.h"
#include <list>

namespace mtf {
    ustring mtf(const ustring &s) {
        ustring result;
        std::list <unsigned char> alphabet;
        for (int i = 0; i < 256; i++) {
            alphabet.push_front(static_cast<unsigned char>(i));
        }
        for (unsigned char c : s) {
            auto it = alphabet.begin();
            int i = 0;
            while (*it != c) {
                it++;
                i++;
            }
            result.push_back(static_cast<unsigned char>(i));
            alphabet.erase(it);
            alphabet.push_front(c);
        }
        return result;
    }

    ustring reverse_mtf(const ustring &s) {
        ustring result;
        std::list <unsigned char> alphabet;
        for (int i = 0; i < 256; i++) {
            alphabet.push_front(static_cast<unsigned char>(i));
        }
        for (unsigned char c : s) {
            auto it = alphabet.begin();
            std::advance(it, static_cast<int>(c));
            unsigned char t = *it;
            result.push_back(*it);
            alphabet.erase(it);
            alphabet.push_front(t);
        }
        return result;
    }

}
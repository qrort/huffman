#ifndef ARCHIVER_MTF_H
#define ARCHIVER_MTF_H
#include <string>

namespace mtf {
    using ustring = std::basic_string<unsigned char>;
    ustring mtf(const ustring &s);
    ustring reverse_mtf(const ustring &s);
}


#endif //ARCHIVER_MTF_H

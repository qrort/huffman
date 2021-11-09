#include "archiver.h"
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    try {
        string type(argv[1]);
        if (type == "-e") {
            archiver::encode(string(argv[2]), string(argv[3]));
        } else if (type == "-d") {
            archiver::decode(string(argv[2]), string(argv[3]));
        }
    } catch (...) {
        throw runtime_error("Wrong query");
    }

    return 0;
}
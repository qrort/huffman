#include "../tree/header/tree-impl.h"

using namespace std;

void encode(string const &input_filename, string const &target_filename) {
	ifstream in(input_filename);
    ofstream out(target_filename);
    // если не смог открыть файлы?

    huffman_tree current;

    current.make(in);
    current.write(out);

    in.clear();
    in.seekg(0);

    current.encode(in, out);
}

void decode(string const &input_filename, string const &target_filename) {
    ifstream in(input_filename);
    ofstream out(target_filename);
    // если не смог открыть файлы?

	huffman_tree current;
    current.read(in);

    current.decode(in, out);
}

int main(int argc, char* argv[]) {
    // а если не передал аргумент?
    string type(argv[1]);
    if (type == "-e") {
        encode(string(argv[2]), string(argv[3]));
    } else if (type == "-d") {
        decode(string(argv[2]), string(argv[3]));
	} else throw runtime_error("Wrong query");
    return 0;
}

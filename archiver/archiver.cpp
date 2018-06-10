#include "../tree/header/tree-impl.h"

using namespace std;

void encode(string input_filename, string target_filename) {
	ifstream in;
    ofstream out;
	
	in.open(input_filename);
    out.open(target_filename);

    huffman_tree current;

    current.make(in);
    current.write(out);

    in.close();
    in.open(input_filename);

    current.encode(in, out);

    in.close();
    out.close();
}

void decode(string input_filename, string target_filename) {
    ifstream in;
    ofstream out;
	
    in.open(input_filename);
    out.open(target_filename);

	huffman_tree current;
    current.read(in);

    current.decode(in, out);

	in.close();
    out.close();
}

int main(int argc, char* argv[]) {
    string type(argv[1]);
    if (type == "-e") {
        encode(string(argv[2]), string(argv[3]));
    } else if (type == "-d") {
        decode(string(argv[2]), string(argv[3]));
	} else throw runtime_error("Wrong query");
    return 0;
}

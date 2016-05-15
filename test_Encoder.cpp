#include "common.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>

using namespace std;

int
main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "./test_Encoder <input file> <output file>\n");
    } else {
        ifstream input(argv[1], ios::binary);
        istreambuf_iterator<char> start(input);
        istreambuf_iterator<char> eof;
        vector<char> vec_in(start, eof);

        Encoder e(2000, 3000, 2);
        Serializer s;

        int c = 0;
        double times = 10000;
        for (int i = 0; i < times; i++) {
            PaddingPackage p(e, vec_in);
            vector<EncodedPackage> vec_ep(e.encode(p));
            vector<vector<char>> vec_s;
            for (const EncodedPackage& ep : vec_ep) {
                vec_s.push_back(s.serialize(ep));
            }

            vec_ep.clear();
            for (const vector<char>& v : vec_s) {
                vec_ep.push_back(s.deserialize(v));
            }
            PaddingPackage pp(e.decode(vec_ep));
            vector<char> vec_out(pp.getRawData().first, pp.getRawData().first + pp.getRawData().second);
            if (vec_in == vec_out) {
                c++;
            }
        }
        std::cout << c / times << std::endl;

        ofstream output(argv[2], ios::binary);
        ostreambuf_iterator<char> out_iter(output);
        //copy(pp.getRawData().first, pp.getRawData().first + pp.getRawData().second, out_iter);
        output.close();
        input.close();
    }
    return 0;
}

#include "common.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>

int
main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "./test_Encoder <input file> <output file>\n");
    } else {
        std::ifstream input(argv[1], std::ios::binary);
        std::istreambuf_iterator<char> start(input);
        std::istreambuf_iterator<char> eof;
        std::vector<char> vec_in(start, eof);

        unsigned c = 0;
        double times = 10000;

        Encoder e;
        Serializer s;
        PaddingPackage p(e, vec_in);
        for (size_t i = 0; i < times; i++) {
            std::vector<EncodedPackage> vec_ep(e.encode(p));
            std::vector<std::vector<char>> vec_s;
            for (const EncodedPackage& ep : vec_ep) {
                vec_s.push_back(s.serialize(ep));
            }

            vec_ep.clear();
            for (const std::vector<char>& v : vec_s) {
                vec_ep.push_back(s.deserialize(v));
            }
            PaddingPackage pp(e.decode(vec_ep));
            std::vector<char> vec_out(pp.getRawData().first, pp.getRawData().first + pp.getRawData().second);
            if (vec_in == vec_out) {
                c++;
            }
        }
        std::cout << c / times << std::endl;

        //std::ofstream output(argv[2], std::ios::binary);
        //std::ostreambuf_iterator<char> out_iter(output);
        //copy(pp.getRawData().first, pp.getRawData().first + pp.getRawData().second, out_iter);
        //output.close();
        input.close();
    }
    return 0;
}

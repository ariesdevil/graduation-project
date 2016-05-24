#include "common.h"

int
main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "./UDPRawServer <sender ip> <receiver ip>\n");
    } else {
        /*
        Encoder e(10, 60, 512, {
                std::make_pair(0, 0),
                std::make_pair(1, 0.1),
                std::make_pair(2, 0.5),
                std::make_pair(3, 0.1667),
                std::make_pair(4, 0.0833),
                std::make_pair(5, 0.05),
                std::make_pair(6, 0.0333),
                std::make_pair(7, 0.0238),
                std::make_pair(8, 0.0179),
                std::make_pair(9, 0.0139),
                std::make_pair(10, 0.0111)}
                );
        */
        Encoder e;
        UDPEncodedClient client(e, argv[1], argv[2]);
        client.run();
    }
    return 0;
}

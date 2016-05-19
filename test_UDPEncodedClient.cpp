#include "common.h"

int
main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "./UDPRawServer <sender ip> <receiver ip>\n");
    } else {
        Encoder e(10, 50, 512, {
                make_pair(0, 0),
                make_pair(1, 0.1),
                make_pair(2, 0.5),
                make_pair(3, 0.1667),
                make_pair(4, 0.0833),
                make_pair(5, 0.05),
                make_pair(6, 0.0333),
                make_pair(7, 0.0238),
                make_pair(8, 0.0179),
                make_pair(9, 0.0139),
                make_pair(10, 0.0111)}
                );

        UDPEncodedClient client(e, argv[1], argv[2]);
        client.run();
    }
    return 0;
}

#include "common.h"

int
main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "./UDPRawServer <sender ip> <receiver ip>\n");
    } else {
        Encoder e;
        UDPEncodedClient client(e, argv[1], argv[2]);
        client.run();
    }
    return 0;
}

#include "common.h"


int
main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "./UDPRawServer <sender ip> <receiver ip>\n");
        return -1;
    } else {
        Encoder e;
        UDPRawServer server(e, argv[1], argv[2]);
        server.run();
    }
    return 0;
}

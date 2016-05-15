#include "common.h"


int
main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "./UDPEncodedServer <sender ip> <receiver ip>\n");
    } else {
        Encoder e;
        UDPEncodedServer server(e, argv[1],argv[2]);
        server.run();
    }
    return 0;
}
